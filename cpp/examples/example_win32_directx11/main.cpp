#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "HttpClient.h"
#include <d3d11.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <ctime>
#include <cstring>
#include <cstdio>
#include <algorithm>

static ID3D11Device*            g_pd3dDevice = nullptr;
static ID3D11DeviceContext*     g_pd3dDeviceContext = nullptr;
static IDXGISwapChain*          g_pSwapChain = nullptr;
static bool                     g_SwapChainOccluded = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView*  g_mainRenderTargetView = nullptr;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int main(int, char**)
{
    ImGui_ImplWin32_EnableDpiAwareness();
    float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Rede Social API - Frontend", WS_OVERLAPPEDWINDOW, 100, 100, (int)(1400 * main_scale), (int)(900 * main_scale), nullptr, nullptr, wc.hInstance, nullptr);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);
    style.FontScaleDpi = main_scale;

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    ImVec4 clear_color = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    
    HttpClient httpClient("http://localhost:9090");
    
    std::string error_text = "";
    std::string success_text = "";
    
    static int default_user_id = 1;
    
    static char nova_postagem[2048] = "";
    static char nova_story[1024] = "";
    static std::string postagens_json = "";
    static std::string stories_json = "";
    static std::string curtidas_json = "";
    static std::string comentarios_json = "";
    static bool refresh_feed = true;
    
    static char busca_usuario[256] = "";
    static std::vector<Usuario> usuarios_encontrados;
    static std::string usuarios_json = "";
    
    static std::vector<Postagem> postagens_lista;
    static std::vector<Stories> stories_lista;
    static std::map<int, Usuario> usuarios_cache;
    static std::queue<std::string> notificacoes;
    static std::vector<Curtida> curtidas_lista;
    static std::vector<Comentario> comentarios_lista;
    
    static int edit_postagem_id = -1;
    static char edit_postagem_conteudo[2048] = "";
    static char edit_postagem_tipo[64] = "texto";
    
    static int edit_story_id = -1;
    static char edit_story_conteudo[1024] = "";
    
    static int edit_curtida_id = -1;
    static int edit_curtida_postagem_id = 0;
    static int edit_curtida_usuario_id = 0;
    
    static int edit_comentario_id = -1;
    static char edit_comentario_conteudo[2048] = "";
    static int edit_comentario_postagem_id = 0;
    static int edit_comentario_usuario_id = 0;
    
    static int edit_usuario_id = -1;
    static char edit_usuario_nome[256] = "";
    static char edit_usuario_email[256] = "";
    static char edit_usuario_dataNasc[256] = "";
    
    static int delete_postagem_id = -1;
    static int delete_story_id = -1;
    static int delete_curtida_id = -1;
    static int delete_comentario_id = -1;
    static int delete_usuario_id = -1;
    
    static int query_postagem_usuario_id = 0;
    static char query_postagem_tipo[64] = "";
    static int query_story_usuario_id = 0;
    static int query_curtida_usuario_id = 0;
    static int query_curtida_postagem_id = 0;
    static int query_comentario_postagem_id = 0;
    
    static char novo_comentario[2048] = "";

    bool done = false;
    while (!done)
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        if (g_SwapChainOccluded && g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
        {
            ::Sleep(10);
            continue;
        }
        g_SwapChainOccluded = false;

        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Rede Social - Teste de API", nullptr, ImGuiWindowFlags_MenuBar);
        
        if (ImGui::BeginMenuBar())
        {
            ImGui::Text("Teste de API - Usuário ID Padrão: %d", default_user_id);
            ImGui::SameLine(ImGui::GetWindowWidth() - 200);
            ImGui::InputInt("ID Usuário", &default_user_id);
            ImGui::EndMenuBar();
        }
            
            if (refresh_feed)
            {
                postagens_json = httpClient.GetPostagens();
                stories_json = httpClient.GetStories();
                curtidas_json = httpClient.GetCurtidas();
                comentarios_json = httpClient.GetComentarios();
                
                postagens_lista = httpClient.ParsePostagens(postagens_json);
                stories_lista = httpClient.ParseStories(stories_json);
                curtidas_lista = httpClient.ParseCurtidas(curtidas_json);
                comentarios_lista = httpClient.ParseComentarios(comentarios_json);
                
                notificacoes.push("Feed atualizado!");
                
                refresh_feed = false;
            }
            
            if (ImGui::BeginTabBar("FeedTabs"))
            {
                if (ImGui::BeginTabItem("Feed"))
                {
                    ImGui::SeparatorText("Criar Nova Postagem");
                    ImGui::InputTextMultiline("##nova_postagem", nova_postagem, sizeof(nova_postagem), ImVec2(-1, 100));
                    if (ImGui::Button("Publicar"))
                    {
                        if (strlen(nova_postagem) > 0)
                        {
                            Postagem p;
                            p.conteudo = nova_postagem;
                            p.tipo = "texto";
                            time_t now = time(0);
                            struct tm timeinfo;
                            localtime_s(&timeinfo, &now);
                            char date_str[32];
                            sprintf_s(date_str, sizeof(date_str), "%04d-%02d-%02d", 
                                timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday);
                            p.data = date_str;
                            p.usuario_id = default_user_id;
                            
                            std::string result = httpClient.CreatePostagem(p);
                            if (!result.empty())
                            {
                                memset(nova_postagem, 0, sizeof(nova_postagem));
                                refresh_feed = true;
                                success_text = "Postagem publicada!";
                            }
                            else
                            {
                                error_text = "Erro ao publicar postagem.";
                            }
                        }
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Atualizar Feed"))
                    {
                        refresh_feed = true;
                    }
                    
                    ImGui::Separator();
                    ImGui::SeparatorText("Buscar Postagens (QUERY)");
                    ImGui::InputInt("ID do Usuário", &query_postagem_usuario_id);
                    ImGui::SameLine();
                    ImGui::InputText("Tipo", query_postagem_tipo, sizeof(query_postagem_tipo));
                    ImGui::SameLine();
                    if (ImGui::Button("Buscar"))
                    {
                        std::string result = httpClient.QueryPostagens(
                            query_postagem_usuario_id > 0 ? query_postagem_usuario_id : -1,
                            strlen(query_postagem_tipo) > 0 ? std::string(query_postagem_tipo) : ""
                        );
                        postagens_json = result;
                        postagens_lista = httpClient.ParsePostagens(postagens_json);
                        notificacoes.push("Busca de postagens realizada!");
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Limpar Filtros"))
                    {
                        query_postagem_usuario_id = 0;
                        memset(query_postagem_tipo, 0, sizeof(query_postagem_tipo));
                        refresh_feed = true;
                    }
                    
                    ImGui::Separator();
                    
                    ImGui::Text("Feed");
                    ImGui::Separator();
                    ImGui::BeginChild("PostagensList", ImVec2(0, 0), true);
                    if (!postagens_lista.empty())
                    {
                        for (const auto& postagem : postagens_lista)
                        {
                            ImGui::PushID(postagem.id_postagem);
                            
                            Usuario autor;
                            if (usuarios_cache.find(postagem.usuario_id) != usuarios_cache.end())
                            {
                                autor = usuarios_cache[postagem.usuario_id];
                            }
                            else
                            {
                                std::string usuario_json = httpClient.GetUsuarioById(postagem.usuario_id);
                                std::vector<Usuario> usuarios_temp = httpClient.ParseUsuarios(usuario_json);
                                if (!usuarios_temp.empty())
                                {
                                    autor = usuarios_temp[0];
                                    usuarios_cache[postagem.usuario_id] = autor;
                                }
                            }
                            
                            ImGui::Separator();
                            ImGui::Text("ID: %ld", postagem.id_postagem);
                            ImGui::Text("Autor: %s", autor.nome.empty() ? postagem.usuario_nome.c_str() : autor.nome.c_str());
                            ImGui::Text("Data: %s", postagem.data.c_str());
                            ImGui::Text("Tipo: %s", postagem.tipo.c_str());
                            ImGui::Spacing();
                            ImGui::TextWrapped("Conteúdo: %s", postagem.conteudo.c_str());
                            ImGui::Spacing();
                            
                            if (ImGui::Button("Editar"))
                            {
                                edit_postagem_id = postagem.id_postagem;
                                strncpy_s(edit_postagem_conteudo, sizeof(edit_postagem_conteudo), postagem.conteudo.c_str(), _TRUNCATE);
                                strncpy_s(edit_postagem_tipo, sizeof(edit_postagem_tipo), postagem.tipo.c_str(), _TRUNCATE);
                                ImGui::OpenPopup("Editar Postagem");
                            }
                            ImGui::SameLine();
                            if (ImGui::Button("Deletar"))
                            {
                                delete_postagem_id = postagem.id_postagem;
                                ImGui::OpenPopup("Deletar Postagem?");
                            }
                            ImGui::Spacing();
                            
                            if (ImGui::BeginPopupModal("Editar Postagem", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
                            {
                                ImGui::Text("Editar Postagem ID: %d", edit_postagem_id);
                                ImGui::Separator();
                                ImGui::InputTextMultiline("Conteúdo", edit_postagem_conteudo, sizeof(edit_postagem_conteudo), ImVec2(400, 100));
                                ImGui::InputText("Tipo", edit_postagem_tipo, sizeof(edit_postagem_tipo));
                                
                                if (ImGui::Button("Salvar", ImVec2(120, 0)))
                                {
                                    Postagem p;
                                    p.id_postagem = edit_postagem_id;
                                    p.conteudo = edit_postagem_conteudo;
                                    p.tipo = edit_postagem_tipo;
                                    p.usuario_id = default_user_id;
                                    time_t now = time(0);
                                    struct tm timeinfo;
                                    localtime_s(&timeinfo, &now);
                                    char date_str[32];
                                    sprintf_s(date_str, sizeof(date_str), "%04d-%02d-%02d", 
                                        timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday);
                                    p.data = date_str;
                                    
                                    std::string result = httpClient.UpdatePostagem(edit_postagem_id, p);
                                    if (!result.empty())
                                    {
                                        refresh_feed = true;
                                        success_text = "Postagem atualizada!";
                                        edit_postagem_id = -1;
                                        ImGui::CloseCurrentPopup();
                                    }
                                    else
                                    {
                                        error_text = "Erro ao atualizar postagem.";
                                    }
                                }
                                ImGui::SameLine();
                                if (ImGui::Button("Cancelar", ImVec2(120, 0)))
                                {
                                    edit_postagem_id = -1;
                                    ImGui::CloseCurrentPopup();
                                }
                                ImGui::EndPopup();
                            }
                            
                            if (ImGui::BeginPopupModal("Deletar Postagem?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
                            {
                                ImGui::Text("Tem certeza que deseja deletar a postagem ID: %d?", delete_postagem_id);
                                ImGui::Separator();
                                if (ImGui::Button("Sim", ImVec2(120, 0)))
                                {
                                    std::string result = httpClient.DeletePostagem(delete_postagem_id);
                                    if (!result.empty() || result == "")
                                    {
                                        refresh_feed = true;
                                        success_text = "Postagem deletada!";
                                        delete_postagem_id = -1;
                                        ImGui::CloseCurrentPopup();
                                    }
                                    else
                                    {
                                        error_text = "Erro ao deletar postagem.";
                                    }
                                }
                                ImGui::SameLine();
                                if (ImGui::Button("Não", ImVec2(120, 0)))
                                {
                                    delete_postagem_id = -1;
                                    ImGui::CloseCurrentPopup();
                                }
                                ImGui::EndPopup();
                            }
                            
                            ImGui::PopID();
                        }
                    }
                    else if (!postagens_json.empty())
                    {
                        ImGui::TextWrapped("%s", postagens_json.c_str());
                    }
                    else
                    {
                        ImGui::Text("Nenhuma postagem ainda. Seja o primeiro a postar!");
                    }
                    ImGui::EndChild();
                    
                    ImGui::EndTabItem();
                }
                
                if (ImGui::BeginTabItem("Stories"))
                {
                    ImGui::SeparatorText("Criar Nova Story");
                    ImGui::InputTextMultiline("##nova_story", nova_story, sizeof(nova_story), ImVec2(-1, 100));
                    if (ImGui::Button("Publicar Story"))
                    {
                        if (strlen(nova_story) > 0)
                        {
                            Stories s;
                            s.conteudo = nova_story;
                            s.usuario_id = default_user_id;
                            
                            std::string result = httpClient.CreateStory(s);
                            if (!result.empty())
                            {
                                memset(nova_story, 0, sizeof(nova_story));
                                refresh_feed = true;
                                success_text = "Story publicada!";
                            }
                            else
                            {
                                error_text = "Erro ao publicar story.";
                            }
                        }
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Atualizar"))
                    {
                        refresh_feed = true;
                    }
                    
                    ImGui::Separator();
                    ImGui::SeparatorText("Buscar Stories (QUERY)");
                    ImGui::InputInt("ID do Usuário", &query_story_usuario_id);
                    ImGui::SameLine();
                    if (ImGui::Button("Buscar Stories"))
                    {
                        std::string result = httpClient.QueryStories(
                            query_story_usuario_id > 0 ? query_story_usuario_id : -1
                        );
                        stories_json = result;
                        stories_lista = httpClient.ParseStories(stories_json);
                        notificacoes.push("Busca de stories realizada!");
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Limpar Filtro"))
                    {
                        query_story_usuario_id = 0;
                        refresh_feed = true;
                    }
                    
                    ImGui::Separator();
                    
                    ImGui::Text("Stories");
                    ImGui::Separator();
                    ImGui::BeginChild("StoriesList", ImVec2(0, 0), true);
                    if (!stories_lista.empty())
                    {
                        for (const auto& story : stories_lista)
                        {
                            ImGui::PushID(story.idStories);
                            
                            ImGui::Separator();
                            ImGui::Text("ID: %d", story.idStories);
                            ImGui::Text("Autor: %s", story.usuario_nome.c_str());
                            ImGui::Spacing();
                            ImGui::TextWrapped("Conteúdo: %s", story.conteudo.c_str());
                            ImGui::Spacing();
                            
                            if (ImGui::Button("Editar"))
                            {
                                edit_story_id = story.idStories;
                                strncpy_s(edit_story_conteudo, sizeof(edit_story_conteudo), story.conteudo.c_str(), _TRUNCATE);
                                ImGui::OpenPopup("Editar Story");
                            }
                            ImGui::SameLine();
                            if (ImGui::Button("Deletar"))
                            {
                                delete_story_id = story.idStories;
                                ImGui::OpenPopup("Deletar Story?");
                            }
                            ImGui::Spacing();
                            
                            if (ImGui::BeginPopupModal("Editar Story", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
                            {
                                ImGui::Text("Editar Story ID: %d", edit_story_id);
                                ImGui::Separator();
                                ImGui::InputTextMultiline("Conteúdo", edit_story_conteudo, sizeof(edit_story_conteudo), ImVec2(400, 100));
                                
                                if (ImGui::Button("Salvar", ImVec2(120, 0)))
                                {
                                    Stories s;
                                    s.idStories = edit_story_id;
                                    s.conteudo = edit_story_conteudo;
                                    s.usuario_id = default_user_id;
                                    
                                    std::string result = httpClient.UpdateStory(edit_story_id, s);
                                    if (!result.empty())
                                    {
                                        refresh_feed = true;
                                        success_text = "Story atualizada!";
                                        edit_story_id = -1;
                                        ImGui::CloseCurrentPopup();
                                    }
                                    else
                                    {
                                        error_text = "Erro ao atualizar story.";
                                    }
                                }
                                ImGui::SameLine();
                                if (ImGui::Button("Cancelar", ImVec2(120, 0)))
                                {
                                    edit_story_id = -1;
                                    ImGui::CloseCurrentPopup();
                                }
                                ImGui::EndPopup();
                            }
                            
                            if (ImGui::BeginPopupModal("Deletar Story?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
                            {
                                ImGui::Text("Tem certeza que deseja deletar a story ID: %d?", delete_story_id);
                                ImGui::Separator();
                                if (ImGui::Button("Sim", ImVec2(120, 0)))
                                {
                                    std::string result = httpClient.DeleteStory(delete_story_id);
                                    if (!result.empty() || result == "")
                                    {
                                        refresh_feed = true;
                                        success_text = "Story deletada!";
                                        delete_story_id = -1;
                                        ImGui::CloseCurrentPopup();
                                    }
                                    else
                                    {
                                        error_text = "Erro ao deletar story.";
                                    }
                                }
                                ImGui::SameLine();
                                if (ImGui::Button("Não", ImVec2(120, 0)))
                                {
                                    delete_story_id = -1;
                                    ImGui::CloseCurrentPopup();
                                }
                                ImGui::EndPopup();
                            }
                            
                            ImGui::PopID();
                        }
                    }
                    else if (!stories_json.empty())
                    {
                        ImGui::TextWrapped("%s", stories_json.c_str());
                    }
                    else
                    {
                        ImGui::Text("Nenhuma story ainda.");
                    }
                    ImGui::EndChild();
                    
                    ImGui::EndTabItem();
                }
                
                if (ImGui::BeginTabItem("Buscar Usuários"))
                {
                    ImGui::SeparatorText("Buscar Usuários");
                    ImGui::InputText("Nome do usuário", busca_usuario, sizeof(busca_usuario));
                    ImGui::SameLine();
                    if (ImGui::Button("Buscar"))
                    {
                        if (strlen(busca_usuario) > 0)
                        {
                            usuarios_json = httpClient.SearchUsuarioByName(busca_usuario);
                            usuarios_encontrados = httpClient.ParseUsuarios(usuarios_json);
                            
                            for (const auto& usuario : usuarios_encontrados)
                            {
                                usuarios_cache[usuario.id_usuario] = usuario;
                            }
                            
                            notificacoes.push("Busca realizada com sucesso!");
                        }
                        else
                        {
                            usuarios_json = httpClient.GetUsuarios();
                            usuarios_encontrados = httpClient.ParseUsuarios(usuarios_json);
                            
                            for (const auto& usuario : usuarios_encontrados)
                            {
                                usuarios_cache[usuario.id_usuario] = usuario;
                            }
                            
                            notificacoes.push("Lista de usuários carregada!");
                        }
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Limpar"))
                    {
                        memset(busca_usuario, 0, sizeof(busca_usuario));
                        usuarios_encontrados.clear();
                        usuarios_json = "";
                    }
                    
                    ImGui::Separator();
                    
                    ImGui::Text("Usuários Encontrados: %d", (int)usuarios_encontrados.size());
                    ImGui::Separator();
                    ImGui::BeginChild("UsuariosList", ImVec2(0, 0), true);
                    
                    if (usuarios_encontrados.empty())
                    {
                        if (usuarios_json.empty())
                        {
                            ImGui::Text("Digite um nome para buscar ou clique em 'Buscar' para ver todos os usuários.");
                        }
                        else
                        {
                            ImGui::Text("Nenhum usuário encontrado (exceto você).");
                        }
                    }
                    else
                    {
                        for (const auto& usuario : usuarios_encontrados)
                        {
                            ImGui::PushID(usuario.id_usuario);
                            
                            ImGui::BeginGroup();
                            ImGui::Text("ID: %d", usuario.id_usuario);
                            ImGui::Text("Nome: %s", usuario.nome.c_str());
                            ImGui::Text("Email: %s", usuario.email.c_str());
                            if (!usuario.dataNasc.empty())
                            {
                                ImGui::Text("Data Nascimento: %s", usuario.dataNasc.c_str());
                            }
                            ImGui::Separator();
                            
                            if (ImGui::Button("Editar"))
                            {
                                edit_usuario_id = usuario.id_usuario;
                                strncpy_s(edit_usuario_nome, sizeof(edit_usuario_nome), usuario.nome.c_str(), _TRUNCATE);
                                strncpy_s(edit_usuario_email, sizeof(edit_usuario_email), usuario.email.c_str(), _TRUNCATE);
                                strncpy_s(edit_usuario_dataNasc, sizeof(edit_usuario_dataNasc), usuario.dataNasc.c_str(), _TRUNCATE);
                                ImGui::OpenPopup("Editar Usuário");
                            }
                            ImGui::SameLine();
                            if (ImGui::Button("Deletar"))
                            {
                                delete_usuario_id = usuario.id_usuario;
                                ImGui::OpenPopup("Deletar Usuário?");
                            }
                            ImGui::EndGroup();
                            
                            if (ImGui::BeginPopupModal("Editar Usuário", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
                            {
                                ImGui::Text("Editar Usuário ID: %d", edit_usuario_id);
                                ImGui::Separator();
                                ImGui::InputText("Nome", edit_usuario_nome, sizeof(edit_usuario_nome));
                                ImGui::InputText("Email", edit_usuario_email, sizeof(edit_usuario_email));
                                ImGui::InputText("Data Nascimento (YYYY-MM-DD)", edit_usuario_dataNasc, sizeof(edit_usuario_dataNasc));
                                
                                if (ImGui::Button("Salvar", ImVec2(120, 0)))
                                {
                                    Usuario u;
                                    u.id_usuario = edit_usuario_id;
                                    u.nome = edit_usuario_nome;
                                    u.email = edit_usuario_email;
                                    u.dataNasc = edit_usuario_dataNasc;
                                    
                                    std::string result = httpClient.UpdateUsuario(edit_usuario_id, u);
                                    if (!result.empty())
                                    {
                                        refresh_feed = true;
                                        success_text = "Usuário atualizado!";
                                        edit_usuario_id = -1;
                                        usuarios_cache.clear();
                                        ImGui::CloseCurrentPopup();
                                    }
                                    else
                                    {
                                        error_text = "Erro ao atualizar usuário.";
                                    }
                                }
                                ImGui::SameLine();
                                if (ImGui::Button("Cancelar", ImVec2(120, 0)))
                                {
                                    edit_usuario_id = -1;
                                    ImGui::CloseCurrentPopup();
                                }
                                ImGui::EndPopup();
                            }
                            
                            if (ImGui::BeginPopupModal("Deletar Usuário?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
                            {
                                ImGui::Text("Tem certeza que deseja deletar o usuário ID: %d?", delete_usuario_id);
                                ImGui::Separator();
                                if (ImGui::Button("Sim", ImVec2(120, 0)))
                                {
                                    std::string result = httpClient.DeleteUsuario(delete_usuario_id);
                                    if (!result.empty() || result == "")
                                    {
                                        refresh_feed = true;
                                        success_text = "Usuário deletado!";
                                        delete_usuario_id = -1;
                                        usuarios_cache.clear();
                                        ImGui::CloseCurrentPopup();
                                    }
                                    else
                                    {
                                        error_text = "Erro ao deletar usuário.";
                                    }
                                }
                                ImGui::SameLine();
                                if (ImGui::Button("Não", ImVec2(120, 0)))
                                {
                                    delete_usuario_id = -1;
                                    ImGui::CloseCurrentPopup();
                                }
                                ImGui::EndPopup();
                            }
                            
                            ImGui::PopID();
                            ImGui::Spacing();
                        }
                    }
                    
                    ImGui::EndChild();
                    ImGui::EndTabItem();
                }
                
                if (ImGui::BeginTabItem("Curtidas"))
                {
                    ImGui::SeparatorText("Gerenciar Curtidas");
                    
                    ImGui::Text("Criar Nova Curtida");
                    ImGui::InputInt("ID da Postagem", &edit_curtida_postagem_id);
                    ImGui::InputInt("ID do Usuário", &edit_curtida_usuario_id);
                    if (ImGui::Button("Criar Curtida"))
                    {
                        if (edit_curtida_postagem_id > 0 && edit_curtida_usuario_id > 0)
                        {
                            Curtida c;
                            c.postagem_id = edit_curtida_postagem_id;
                            c.usuario_id = edit_curtida_usuario_id;
                            time_t now = time(0);
                            struct tm timeinfo;
                            localtime_s(&timeinfo, &now);
                            char date_str[32];
                            sprintf_s(date_str, sizeof(date_str), "%04d-%02d-%02d", 
                                timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday);
                            c.data_curtida = date_str;
                            
                            std::string result = httpClient.CreateCurtida(c);
                            if (!result.empty())
                            {
                                refresh_feed = true;
                                success_text = "Curtida criada!";
                                edit_curtida_postagem_id = 0;
                                edit_curtida_usuario_id = 0;
                            }
                            else
                            {
                                error_text = "Erro ao criar curtida.";
                            }
                        }
                        else
                        {
                            error_text = "Preencha os IDs corretamente!";
                        }
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Atualizar Lista"))
                    {
                        refresh_feed = true;
                    }
                    
                    ImGui::Separator();
                    ImGui::SeparatorText("Buscar Curtidas (QUERY)");
                    ImGui::InputInt("ID do Usuário", &query_curtida_usuario_id);
                    ImGui::SameLine();
                    ImGui::InputInt("ID da Postagem", &query_curtida_postagem_id);
                    ImGui::SameLine();
                    if (ImGui::Button("Buscar Curtidas"))
                    {
                        std::string result = httpClient.QueryCurtidas(
                            query_curtida_usuario_id > 0 ? query_curtida_usuario_id : -1,
                            query_curtida_postagem_id > 0 ? query_curtida_postagem_id : -1
                        );
                        curtidas_json = result;
                        curtidas_lista = httpClient.ParseCurtidas(curtidas_json);
                        notificacoes.push("Busca de curtidas realizada!");
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Limpar Filtros"))
                    {
                        query_curtida_usuario_id = 0;
                        query_curtida_postagem_id = 0;
                        refresh_feed = true;
                    }
                    
                    ImGui::Separator();
                    ImGui::Text("Lista de Curtidas");
                    ImGui::Separator();
                    ImGui::BeginChild("CurtidasList", ImVec2(0, 0), true);
                    
                    if (!curtidas_lista.empty())
                    {
                        for (const auto& curtida : curtidas_lista)
                        {
                            ImGui::PushID(curtida.id_curtida);
                            
                            ImGui::Separator();
                            ImGui::Text("ID: %ld", curtida.id_curtida);
                            ImGui::Text("Usuário: %s (ID: %d)", curtida.usuario_nome.c_str(), curtida.usuario_id);
                            ImGui::Text("Postagem ID: %ld", curtida.postagem_id);
                            ImGui::Text("Data: %s", curtida.data_curtida.c_str());
                            ImGui::Spacing();
                            
                            if (ImGui::Button("Editar"))
                            {
                                edit_curtida_id = curtida.id_curtida;
                                edit_curtida_postagem_id = curtida.postagem_id;
                                edit_curtida_usuario_id = curtida.usuario_id;
                                ImGui::OpenPopup("Editar Curtida");
                            }
                            ImGui::SameLine();
                            if (ImGui::Button("Deletar"))
                            {
                                delete_curtida_id = curtida.id_curtida;
                                ImGui::OpenPopup("Deletar Curtida?");
                            }
                            ImGui::Spacing();
                            
                            if (ImGui::BeginPopupModal("Editar Curtida", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
                            {
                                ImGui::Text("Editar Curtida ID: %d", edit_curtida_id);
                                ImGui::Separator();
                                ImGui::InputInt("ID da Postagem", &edit_curtida_postagem_id);
                                ImGui::InputInt("ID do Usuário", &edit_curtida_usuario_id);
                                
                                if (ImGui::Button("Salvar", ImVec2(120, 0)))
                                {
                                    Curtida c;
                                    c.id_curtida = edit_curtida_id;
                                    c.postagem_id = edit_curtida_postagem_id;
                                    c.usuario_id = edit_curtida_usuario_id;
                                    time_t now = time(0);
                                    struct tm timeinfo;
                                    localtime_s(&timeinfo, &now);
                                    char date_str[32];
                                    sprintf_s(date_str, sizeof(date_str), "%04d-%02d-%02d", 
                                        timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday);
                                    c.data_curtida = date_str;
                                    
                                    std::string result = httpClient.UpdateCurtida(edit_curtida_id, c);
                                    if (!result.empty())
                                    {
                                        refresh_feed = true;
                                        success_text = "Curtida atualizada!";
                                        edit_curtida_id = -1;
                                        ImGui::CloseCurrentPopup();
                                    }
                                    else
                                    {
                                        error_text = "Erro ao atualizar curtida.";
                                    }
                                }
                                ImGui::SameLine();
                                if (ImGui::Button("Cancelar", ImVec2(120, 0)))
                                {
                                    edit_curtida_id = -1;
                                    ImGui::CloseCurrentPopup();
                                }
                                ImGui::EndPopup();
                            }
                            
                            if (ImGui::BeginPopupModal("Deletar Curtida?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
                            {
                                ImGui::Text("Tem certeza que deseja deletar a curtida ID: %d?", delete_curtida_id);
                                ImGui::Separator();
                                if (ImGui::Button("Sim", ImVec2(120, 0)))
                                {
                                    std::string result = httpClient.DeleteCurtida(delete_curtida_id);
                                    if (!result.empty() || result == "")
                                    {
                                        refresh_feed = true;
                                        success_text = "Curtida deletada!";
                                        delete_curtida_id = -1;
                                        ImGui::CloseCurrentPopup();
                                    }
                                    else
                                    {
                                        error_text = "Erro ao deletar curtida.";
                                    }
                                }
                                ImGui::SameLine();
                                if (ImGui::Button("Não", ImVec2(120, 0)))
                                {
                                    delete_curtida_id = -1;
                                    ImGui::CloseCurrentPopup();
                                }
                                ImGui::EndPopup();
                            }
                            
                            ImGui::PopID();
                        }
                    }
                    else if (!curtidas_json.empty())
                    {
                        ImGui::TextWrapped("%s", curtidas_json.c_str());
                    }
                    else
                    {
                        ImGui::Text("Nenhuma curtida ainda.");
                    }
                    
                    ImGui::EndChild();
                    ImGui::EndTabItem();
                }
                
                if (ImGui::BeginTabItem("Comentários"))
                {
                    ImGui::SeparatorText("Criar Novo Comentário");
                    ImGui::InputTextMultiline("##novo_comentario", novo_comentario, sizeof(novo_comentario), ImVec2(-1, 100));
                    ImGui::InputInt("ID da Postagem", &edit_comentario_postagem_id);
                    ImGui::InputInt("ID do Usuário", &edit_comentario_usuario_id);
                    if (ImGui::Button("Publicar Comentário"))
                    {
                        if (strlen(novo_comentario) > 0 && edit_comentario_postagem_id > 0 && edit_comentario_usuario_id > 0)
                        {
                            Comentario c;
                            c.conteudo = novo_comentario;
                            
                            std::string result = httpClient.CreateComentario(edit_comentario_postagem_id, edit_comentario_usuario_id, c);
                            if (!result.empty())
                            {
                                memset(novo_comentario, 0, sizeof(novo_comentario));
                                refresh_feed = true;
                                success_text = "Comentário publicado!";
                            }
                            else
                            {
                                error_text = "Erro ao publicar comentário.";
                            }
                        }
                        else
                        {
                            error_text = "Preencha todos os campos!";
                        }
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Atualizar Lista"))
                    {
                        refresh_feed = true;
                    }
                    
                    ImGui::Separator();
                    ImGui::SeparatorText("Buscar Comentários (QUERY)");
                    ImGui::InputInt("ID da Postagem", &query_comentario_postagem_id);
                    ImGui::SameLine();
                    if (ImGui::Button("Buscar Comentários"))
                    {
                        std::string result = httpClient.QueryComentarios(
                            query_comentario_postagem_id > 0 ? query_comentario_postagem_id : -1
                        );
                        comentarios_json = result;
                        comentarios_lista = httpClient.ParseComentarios(comentarios_json);
                        notificacoes.push("Busca de comentários realizada!");
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Limpar Filtro"))
                    {
                        query_comentario_postagem_id = 0;
                        refresh_feed = true;
                    }
                    
                    ImGui::Separator();
                    ImGui::Text("Lista de Comentários");
                    ImGui::Separator();
                    ImGui::BeginChild("ComentariosList", ImVec2(0, 0), true);
                    
                    if (!comentarios_lista.empty())
                    {
                        for (const auto& comentario : comentarios_lista)
                        {
                            ImGui::PushID(comentario.id_comentario);
                            
                            ImGui::Separator();
                            ImGui::Text("ID: %d", comentario.id_comentario);
                            ImGui::Text("Usuário: %s (ID: %d)", comentario.usuario_nome.c_str(), comentario.usuario_id);
                            ImGui::Text("Postagem ID: %ld", comentario.postagem_id);
                            ImGui::Text("Data: %s", comentario.data_comentario.c_str());
                            ImGui::Spacing();
                            ImGui::TextWrapped("Conteúdo: %s", comentario.conteudo.c_str());
                            ImGui::Spacing();
                            
                            if (ImGui::Button("Editar"))
                            {
                                edit_comentario_id = comentario.id_comentario;
                                strncpy_s(edit_comentario_conteudo, sizeof(edit_comentario_conteudo), comentario.conteudo.c_str(), _TRUNCATE);
                                edit_comentario_postagem_id = comentario.postagem_id;
                                edit_comentario_usuario_id = comentario.usuario_id;
                                ImGui::OpenPopup("Editar Comentário");
                            }
                            ImGui::SameLine();
                            if (ImGui::Button("Deletar"))
                            {
                                delete_comentario_id = comentario.id_comentario;
                                ImGui::OpenPopup("Deletar Comentário?");
                            }
                            ImGui::Spacing();
                            
                            if (ImGui::BeginPopupModal("Editar Comentário", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
                            {
                                ImGui::Text("Editar Comentário ID: %d", edit_comentario_id);
                                ImGui::Separator();
                                ImGui::InputTextMultiline("Conteúdo", edit_comentario_conteudo, sizeof(edit_comentario_conteudo), ImVec2(400, 100));
                                
                                if (ImGui::Button("Salvar", ImVec2(120, 0)))
                                {
                                    Comentario c;
                                    c.id_comentario = edit_comentario_id;
                                    c.conteudo = edit_comentario_conteudo;
                                    
                                    std::string result = httpClient.UpdateComentario(edit_comentario_id, c);
                                    if (!result.empty())
                                    {
                                        refresh_feed = true;
                                        success_text = "Comentário atualizado!";
                                        edit_comentario_id = -1;
                                        ImGui::CloseCurrentPopup();
                                    }
                                    else
                                    {
                                        error_text = "Erro ao atualizar comentário.";
                                    }
                                }
                                ImGui::SameLine();
                                if (ImGui::Button("Cancelar", ImVec2(120, 0)))
                                {
                                    edit_comentario_id = -1;
                                    ImGui::CloseCurrentPopup();
                                }
                                ImGui::EndPopup();
                            }
                            
                            if (ImGui::BeginPopupModal("Deletar Comentário?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
                            {
                                ImGui::Text("Tem certeza que deseja deletar o comentário ID: %d?", delete_comentario_id);
                                ImGui::Separator();
                                if (ImGui::Button("Sim", ImVec2(120, 0)))
                                {
                                    std::string result = httpClient.DeleteComentario(delete_comentario_id);
                                    if (!result.empty() || result == "")
                                    {
                                        refresh_feed = true;
                                        success_text = "Comentário deletado!";
                                        delete_comentario_id = -1;
                                        ImGui::CloseCurrentPopup();
                                    }
                                    else
                                    {
                                        error_text = "Erro ao deletar comentário.";
                                    }
                                }
                                ImGui::SameLine();
                                if (ImGui::Button("Não", ImVec2(120, 0)))
                                {
                                    delete_comentario_id = -1;
                                    ImGui::CloseCurrentPopup();
                                }
                                ImGui::EndPopup();
                            }
                            
                            ImGui::PopID();
                        }
                    }
                    else if (!comentarios_json.empty())
                    {
                        ImGui::TextWrapped("%s", comentarios_json.c_str());
                    }
                    else
                    {
                        ImGui::Text("Nenhum comentário ainda.");
                    }
                    
                    ImGui::EndChild();
                    ImGui::EndTabItem();
                }
                
                if (ImGui::BeginTabItem("Teste API"))
                {
                    ImGui::SeparatorText("Teste de Rotas da API");
                    
                    static int test_id = 0;
                    static char test_response[4096] = "";
                    
                    ImGui::Text("Esta aba permite testar todas as rotas da API diretamente.");
                    ImGui::Separator();
                    
                    ImGui::Text("=== USUÁRIOS ===");
                    if (ImGui::Button("GET /usuarios"))
                    {
                        std::string result = httpClient.GetUsuarios();
                        strncpy_s(test_response, sizeof(test_response), result.c_str(), _TRUNCATE);
                        notificacoes.push("GET /usuarios executado");
                    }
                    ImGui::SameLine();
                    ImGui::InputInt("ID Usuário", &test_id);
                    ImGui::SameLine();
                    if (ImGui::Button("GET /usuarios/{id}"))
                    {
                        std::string result = httpClient.GetUsuarioById(test_id);
                        strncpy_s(test_response, sizeof(test_response), result.c_str(), _TRUNCATE);
                        notificacoes.push("GET /usuarios/{id} executado");
                    }
                    
                    ImGui::Text("=== POSTAGENS ===");
                    if (ImGui::Button("GET /postagens"))
                    {
                        std::string result = httpClient.GetPostagens();
                        strncpy_s(test_response, sizeof(test_response), result.c_str(), _TRUNCATE);
                        notificacoes.push("GET /postagens executado");
                    }
                    ImGui::SameLine();
                    ImGui::InputInt("ID Postagem", &test_id);
                    ImGui::SameLine();
                    if (ImGui::Button("GET /postagens/{id}"))
                    {
                        std::string result = httpClient.GetPostagemById(test_id);
                        strncpy_s(test_response, sizeof(test_response), result.c_str(), _TRUNCATE);
                        notificacoes.push("GET /postagens/{id} executado");
                    }
                    
                    ImGui::Text("=== STORIES ===");
                    if (ImGui::Button("GET /stories"))
                    {
                        std::string result = httpClient.GetStories();
                        strncpy_s(test_response, sizeof(test_response), result.c_str(), _TRUNCATE);
                        notificacoes.push("GET /stories executado");
                    }
                    ImGui::SameLine();
                    ImGui::InputInt("ID Story", &test_id);
                    ImGui::SameLine();
                    if (ImGui::Button("GET /stories/{id}"))
                    {
                        std::string result = httpClient.GetStoryById(test_id);
                        strncpy_s(test_response, sizeof(test_response), result.c_str(), _TRUNCATE);
                        notificacoes.push("GET /stories/{id} executado");
                    }
                    
                    ImGui::Text("=== CURTIDAS ===");
                    if (ImGui::Button("GET /curtidas"))
                    {
                        std::string result = httpClient.GetCurtidas();
                        strncpy_s(test_response, sizeof(test_response), result.c_str(), _TRUNCATE);
                        notificacoes.push("GET /curtidas executado");
                    }
                    ImGui::SameLine();
                    ImGui::InputInt("ID Curtida", &test_id);
                    ImGui::SameLine();
                    if (ImGui::Button("GET /curtidas/{id}"))
                    {
                        std::string result = httpClient.GetCurtidaById(test_id);
                        strncpy_s(test_response, sizeof(test_response), result.c_str(), _TRUNCATE);
                        notificacoes.push("GET /curtidas/{id} executado");
                    }
                    
                    ImGui::Text("=== COMENTÁRIOS ===");
                    if (ImGui::Button("GET /comentarios"))
                    {
                        std::string result = httpClient.GetComentarios();
                        strncpy_s(test_response, sizeof(test_response), result.c_str(), _TRUNCATE);
                        notificacoes.push("GET /comentarios executado");
                    }
                    ImGui::SameLine();
                    ImGui::InputInt("ID Comentário", &test_id);
                    ImGui::SameLine();
                    if (ImGui::Button("GET /comentarios/{id}"))
                    {
                        std::string result = httpClient.GetComentarioById(test_id);
                        strncpy_s(test_response, sizeof(test_response), result.c_str(), _TRUNCATE);
                        notificacoes.push("GET /comentarios/{id} executado");
                    }
                    
                    ImGui::Separator();
                    ImGui::Text("Resposta da API:");
                    ImGui::InputTextMultiline("##response", test_response, sizeof(test_response), ImVec2(-1, 200), ImGuiInputTextFlags_ReadOnly);
                    
                    ImGui::EndTabItem();
                }
                
                ImGui::EndTabBar();
            }
            
            if (!notificacoes.empty())
            {
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Text("Notificações:");
                while (!notificacoes.empty())
                {
                    ImGui::TextColored(ImVec4(0.0f, 0.8f, 1.0f, 1.0f), "• %s", notificacoes.front().c_str());
                    notificacoes.pop();
                }
            }
            
            if (!error_text.empty())
            {
                ImGui::Spacing();
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", error_text.c_str());
            }
            if (!success_text.empty())
            {
                ImGui::Spacing();
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%s", success_text.c_str());
            }
            
            ImGui::End();

        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        HRESULT hr = g_pSwapChain->Present(1, 0);
        g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

bool CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam);
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
