#define CURL_STATICLIB
#include "HttpClient.h"
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cctype>
#include "curl/curl.h"

#ifdef _DEBUG
#pragma comment(lib, "curl/libcurl_a_debug.lib")
#else
#pragma comment(lib, "curl/libcurl_a.lib")
#endif
#pragma comment(lib, "Normaliz.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Wldap32.lib")
#pragma comment(lib, "Crypt32.lib")

HttpClient::HttpClient(const std::string& baseUrl) : baseUrl(baseUrl) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

size_t HttpClient::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
    }
    catch (std::bad_alloc& e) {
        return 0;
    }
    return newLength;
}

std::string EscapeJsonString(const std::string& str) {
    std::string escaped;
    for (char c : str) {
        switch (c) {
            case '"': escaped += "\\\""; break;
            case '\\': escaped += "\\\\"; break;
            case '\n': escaped += "\\n"; break;
            case '\r': escaped += "\\r"; break;
            case '\t': escaped += "\\t"; break;
            default: escaped += c; break;
        }
    }
    return escaped;
}

std::string ExtractJsonStringValue(const std::string& json, const std::string& key) {
    std::string searchKey = "\"" + key + "\":\"";
    size_t pos = json.find(searchKey);
    if (pos != std::string::npos) {
        pos += searchKey.length();
        size_t end = json.find("\"", pos);
        if (end != std::string::npos) {
            return json.substr(pos, end - pos);
        }
    }
    return "";
}

int ExtractJsonIntValue(const std::string& json, const std::string& key) {
    std::string searchKey = "\"" + key + "\":";
    size_t pos = json.find(searchKey);
    if (pos != std::string::npos) {
        pos += searchKey.length();
        while (pos < json.length() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
        size_t end = json.find_first_of(",}", pos);
        if (end != std::string::npos) {
            std::string numStr = json.substr(pos, end - pos);
            try {
                return std::stoi(numStr);
            } catch (...) {
                return 0;
            }
        }
    }
    return 0;
}

std::string HttpClient::MakeRequest(const std::string& method, const std::string& endpoint, const std::string& body) {
    CURL* curl;
    CURLcode res;
    std::string response_string;
    
    curl = curl_easy_init();
    if (!curl) {
        lastError = "Falha ao inicializar curl";
        return "";
    }
    
    std::string url = baseUrl + endpoint;
    
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
    
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    
    if (method == "GET") {
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    }
    else if (method == "POST") {
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        if (!body.empty()) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        }
    }
    else if (method == "PUT") {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        if (!body.empty()) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        }
    }
    else if (method == "DELETE") {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    }
    
    res = curl_easy_perform(curl);
    
    if (res != CURLE_OK) {
        lastError = std::string("curl_easy_perform() failed: ") + curl_easy_strerror(res);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        return "";
    }
    
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    
    lastError = "";
    return response_string;
}

std::string HttpClient::GetUsuarios() {
    return MakeRequest("GET", "/usuarios/get");
}

std::string HttpClient::GetUsuarioById(int id) {
    return MakeRequest("GET", "/usuarios/id/" + std::to_string(id));
}

std::string HttpClient::CreateUsuario(const Usuario& usuario) {
    std::ostringstream json;
    json << "{";
    json << "\"nome\":\"" << EscapeJsonString(usuario.nome) << "\",";
    json << "\"email\":\"" << EscapeJsonString(usuario.email) << "\",";
    json << "\"senha\":\"" << EscapeJsonString(usuario.senha) << "\",";
    json << "\"dataNasc\":\"" << EscapeJsonString(usuario.dataNasc) << "\"";
    json << "}";
    return MakeRequest("POST", "/usuarios/post", json.str());
}

std::string HttpClient::UpdateUsuario(int id, const Usuario& usuario) {
    std::ostringstream json;
    json << "{";
    json << "\"id_usuario\":" << id << ",";
    json << "\"nome\":\"" << EscapeJsonString(usuario.nome) << "\",";
    json << "\"email\":\"" << EscapeJsonString(usuario.email) << "\",";
    json << "\"senha\":\"" << EscapeJsonString(usuario.senha) << "\",";
    json << "\"dataNasc\":\"" << EscapeJsonString(usuario.dataNasc) << "\"";
    json << "}";
    return MakeRequest("PUT", "/usuarios/alter" + std::to_string(id), json.str());
}

std::string HttpClient::DeleteUsuario(int id) {
    return MakeRequest("DELETE", "/usuarios/dell" + std::to_string(id));
}

std::string HttpClient::SearchUsuarioByName(const std::string& nome) {
    std::string encoded;
    for (char c : nome) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encoded += c;
        } else {
            char hex[4];
            sprintf_s(hex, sizeof(hex), "%%%02X", (unsigned char)c);
            encoded += hex;
        }
    }
    return MakeRequest("GET", "/usuarios?nome=" + encoded);
}

std::string HttpClient::GetPostagens() {
    return MakeRequest("GET", "/posts");
}

std::string HttpClient::GetPostagemById(int id) {
    return MakeRequest("GET", "/posts/" + std::to_string(id));
}

std::string HttpClient::CreatePostagem(const Postagem& postagem) {
    std::ostringstream json;
    json << "{";
    json << "\"conteudo\":\"" << EscapeJsonString(postagem.conteudo) << "\",";
    json << "\"tipo\":\"" << EscapeJsonString(postagem.tipo) << "\",";
    json << "\"data\":\"" << EscapeJsonString(postagem.data) << "\",";
    json << "\"usuario\":{\"id_usuario\":" << postagem.usuario_id << "}";
    json << "}";
    return MakeRequest("POST", "/posts/Post", json.str());
}

std::string HttpClient::UpdatePostagem(int id, const Postagem& postagem) {
    std::ostringstream json;
    json << "{";
    json << "\"id_postagem\":" << id << ",";
    json << "\"conteudo\":\"" << EscapeJsonString(postagem.conteudo) << "\",";
    json << "\"tipo\":\"" << EscapeJsonString(postagem.tipo) << "\",";
    json << "\"data\":\"" << EscapeJsonString(postagem.data) << "\",";
    json << "\"usuario\":{\"id_usuario\":" << postagem.usuario_id << "}";
    json << "}";
    return MakeRequest("PUT", "/posts/alter" + std::to_string(id), json.str());
}

std::string HttpClient::DeletePostagem(int id) {
    return MakeRequest("DELETE", "/posts/delete" + std::to_string(id));
}

std::string HttpClient::QueryPostagens(int usuario_id, const std::string& tipo) {
    std::string endpoint = "/posts";
    std::string query = "";
    
    if (usuario_id > 0) {
        query += "?usuario_id=" + std::to_string(usuario_id);
    }
    if (!tipo.empty()) {
        if (query.empty()) {
            query += "?tipo=";
        } else {
            query += "&tipo=";
        }
        std::string encoded;
        for (char c : tipo) {
            if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
                encoded += c;
            } else {
                char hex[4];
                sprintf_s(hex, sizeof(hex), "%%%02X", (unsigned char)c);
                encoded += hex;
            }
        }
        query += encoded;
    }
    
    return MakeRequest("GET", endpoint + query);
}

std::string HttpClient::GetStories() {
    return MakeRequest("GET", "/Stories/AllStories");
}

std::string HttpClient::GetStoryById(int id) {
    return MakeRequest("GET", "/Stories/" + std::to_string(id));
}

std::string HttpClient::CreateStory(const Stories& story) {
    std::ostringstream json;
    json << "{";
    json << "\"conteudo\":\"" << EscapeJsonString(story.conteudo) << "\",";
    json << "\"usuario\":{\"id_usuario\":" << story.usuario_id << "}";
    json << "}";
    return MakeRequest("POST", "/Stories/post", json.str());
}

std::string HttpClient::UpdateStory(int id, const Stories& story) {
    std::ostringstream json;
    json << "{";
    json << "\"idStories\":" << id << ",";
    json << "\"conteudo\":\"" << EscapeJsonString(story.conteudo) << "\",";
    json << "\"usuario\":{\"id_usuario\":" << story.usuario_id << "}";
    json << "}";
    return MakeRequest("PUT", "/Stories/alter" + std::to_string(id), json.str());
}

std::string HttpClient::DeleteStory(int id) {
    return MakeRequest("DELETE", "/Stories/DeletarStories" + std::to_string(id));
}

std::string HttpClient::QueryStories(int usuario_id) {
    std::string endpoint = "/Stories/AllStories";
    if (usuario_id > 0) {
        endpoint += "?usuario_id=" + std::to_string(usuario_id);
    }
    return MakeRequest("GET", endpoint);
}

std::string HttpClient::GetCurtidas() {
    return MakeRequest("GET", "/curtida");
}

std::string HttpClient::GetCurtidaById(int id) {
    return MakeRequest("GET", "/curtida/" + std::to_string(id));
}

std::string HttpClient::CreateCurtida(const Curtida& curtida) {
    std::ostringstream json;
    json << "{";
    json << "\"data_curtida\":\"" << EscapeJsonString(curtida.data_curtida) << "\",";
    json << "\"usuario\":{\"id_usuario\":" << curtida.usuario_id << "},";
    json << "\"postagem\":{\"id_postagem\":" << curtida.postagem_id << "}";
    json << "}";
    return MakeRequest("POST", "/curtida", json.str());
}

std::string HttpClient::UpdateCurtida(int id, const Curtida& curtida) {
    std::ostringstream json;
    json << "{";
    json << "\"id_curtida\":" << id << ",";
    json << "\"data_curtida\":\"" << EscapeJsonString(curtida.data_curtida) << "\",";
    json << "\"usuario\":{\"id_usuario\":" << curtida.usuario_id << "},";
    json << "\"postagem\":{\"id_postagem\":" << curtida.postagem_id << "}";
    json << "}";
    return MakeRequest("PUT", "/curtida/alter" + std::to_string(id), json.str());
}

std::string HttpClient::DeleteCurtida(int id) {
    return MakeRequest("DELETE", "/curtida/delete" + std::to_string(id));
}

std::string HttpClient::QueryCurtidas(int usuario_id, int postagem_id) {
    std::string endpoint = "/curtida";
    std::string query = "";
    
    if (usuario_id > 0) {
        query += "?usuario_id=" + std::to_string(usuario_id);
    }
    if (postagem_id > 0) {
        if (query.empty()) {
            query += "?postagem_id=" + std::to_string(postagem_id);
        } else {
            query += "&postagem_id=" + std::to_string(postagem_id);
        }
    }
    
    return MakeRequest("GET", endpoint + query);
}

std::string HttpClient::GetComentarios() {
    return MakeRequest("GET", "/comentarios");
}

std::string HttpClient::GetComentarioById(int id) {
    return MakeRequest("GET", "/comentarios/" + std::to_string(id));
}

std::string HttpClient::QueryComentarios(int postagem_id) {
    std::string endpoint = "/comentarios";
    if (postagem_id > 0) {
        endpoint += "/postagem/" + std::to_string(postagem_id);
    }
    return MakeRequest("GET", endpoint);
}

std::string HttpClient::CreateComentario(int postagem_id, int usuario_id, const Comentario& comentario) {
    std::ostringstream json;
    json << "{";
    json << "\"conteudo\":\"" << EscapeJsonString(comentario.conteudo) << "\"";
    json << "}";
    
    std::string endpoint = "/comentarios?idPostagem=" + std::to_string(postagem_id) + 
                          "&idUsuario=" + std::to_string(usuario_id);
    return MakeRequest("POST", endpoint, json.str());
}

std::string HttpClient::UpdateComentario(int id, const Comentario& comentario) {
    std::ostringstream json;
    json << "{";
    json << "\"id_comentario\":" << id << ",";
    json << "\"conteudo\":\"" << EscapeJsonString(comentario.conteudo) << "\"";
    json << "}";
    return MakeRequest("PUT", "/comentarios/" + std::to_string(id), json.str());
}

std::string HttpClient::DeleteComentario(int id) {
    return MakeRequest("DELETE", "/comentarios/" + std::to_string(id));
}

Usuario HttpClient::Login(const std::string& email, const std::string& senha) {
    Usuario usuario;
    std::string response = GetUsuarios();
    
    size_t pos = 0;
    while ((pos = response.find("\"email\":\"", pos)) != std::string::npos) {
        pos += 9;
        size_t email_end = response.find("\"", pos);
        if (email_end == std::string::npos) break;
        
        std::string found_email = response.substr(pos, email_end - pos);
        
        if (found_email == email) {
            size_t senha_pos = response.find("\"senha\":\"", email_end);
            if (senha_pos != std::string::npos) {
                senha_pos += 9;
                size_t senha_end = response.find("\"", senha_pos);
                if (senha_end != std::string::npos) {
                    std::string found_senha = response.substr(senha_pos, senha_end - senha_pos);
                    if (found_senha == senha) {
                        size_t id_pos = response.rfind("\"id_usuario\":", senha_pos);
                        if (id_pos != std::string::npos && id_pos < senha_pos) {
                            id_pos += 13;
                            size_t id_end = response.find_first_of(",}", id_pos);
                            if (id_end != std::string::npos) {
                                usuario.id_usuario = std::stoi(response.substr(id_pos, id_end - id_pos));
                            }
                        }
                        
                        size_t nome_pos = response.find("\"nome\":\"", email_end);
                        if (nome_pos != std::string::npos && nome_pos < senha_pos) {
                            nome_pos += 8;
                            size_t nome_end = response.find("\"", nome_pos);
                            if (nome_end != std::string::npos) {
                                usuario.nome = response.substr(nome_pos, nome_end - nome_pos);
                            }
                        }
                        
                        usuario.email = email;
                        usuario.senha = senha;
                        return usuario;
                    }
                }
            }
        }
        pos = email_end;
    }
    
    return usuario;
}

bool HttpClient::Register(const Usuario& usuario) {
    std::string response = CreateUsuario(usuario);
    return !response.empty() && response.find("id_usuario") != std::string::npos;
}

std::vector<Usuario> HttpClient::ParseUsuarios(const std::string& json) {
    std::vector<Usuario> usuarios;
    
    if (json.empty()) {
        return usuarios;
    }
    
    size_t pos = 0;
    int brace_count = 0;
    size_t obj_start = 0;
    
    for (size_t i = 0; i < json.length(); i++) {
        if (json[i] == '{') {
            if (brace_count == 0) {
                obj_start = i;
            }
            brace_count++;
        } else if (json[i] == '}') {
            brace_count--;
            if (brace_count == 0) {
                std::string obj = json.substr(obj_start, i - obj_start + 1);
                
                Usuario u;
                u.id_usuario = ExtractJsonIntValue(obj, "id_usuario");
                u.nome = ExtractJsonStringValue(obj, "nome");
                u.email = ExtractJsonStringValue(obj, "email");
                u.dataNasc = ExtractJsonStringValue(obj, "dataNasc");
                
                if (u.id_usuario > 0) {
                    usuarios.push_back(u);
                }
            }
        }
    }
    
    return usuarios;
}

std::vector<Postagem> HttpClient::ParsePostagens(const std::string& json) {
    std::vector<Postagem> postagens;
    
    if (json.empty()) {
        return postagens;
    }
    
    size_t pos = 0;
    int brace_count = 0;
    size_t obj_start = 0;
    
    for (size_t i = 0; i < json.length(); i++) {
        if (json[i] == '{') {
            if (brace_count == 0) {
                obj_start = i;
            }
            brace_count++;
        } else if (json[i] == '}') {
            brace_count--;
            if (brace_count == 0) {
                std::string obj = json.substr(obj_start, i - obj_start + 1);
                
                Postagem p;
                p.id_postagem = ExtractJsonIntValue(obj, "idPostagem");
                if (p.id_postagem == 0) {
                    p.id_postagem = ExtractJsonIntValue(obj, "id_postagem");
                }
                p.conteudo = ExtractJsonStringValue(obj, "conteudo");
                p.tipo = ExtractJsonStringValue(obj, "tipo");
                
                std::string dataStr = ExtractJsonStringValue(obj, "dataPublicacao");
                if (dataStr.empty()) {
                    dataStr = ExtractJsonStringValue(obj, "data_publicacao");
                }
                if (dataStr.empty()) {
                    dataStr = ExtractJsonStringValue(obj, "data");
                }
                p.data = dataStr;
                
                int usuarioId = ExtractJsonIntValue(obj, "idUsuario");
                if (usuarioId == 0) {
                    usuarioId = ExtractJsonIntValue(obj, "id_usuario");
                }
                if (usuarioId == 0) {
                    size_t usuarioPos = obj.find("\"usuario\":");
                    if (usuarioPos != std::string::npos) {
                        size_t idPos = obj.find("\"idUsuario\":", usuarioPos);
                        if (idPos == std::string::npos) {
                            idPos = obj.find("\"id_usuario\":", usuarioPos);
                        }
                        if (idPos != std::string::npos) {
                            idPos += (obj.find("\"idUsuario\":", usuarioPos) != std::string::npos) ? 13 : 14;
                            size_t idEnd = obj.find_first_of(",}", idPos);
                            if (idEnd != std::string::npos) {
                                usuarioId = std::stoi(obj.substr(idPos, idEnd - idPos));
                            }
                        }
                    }
                }
                p.usuario_id = usuarioId;
                
                p.usuario_nome = ExtractJsonStringValue(obj, "usuario_nome");
                
                if (p.id_postagem > 0) {
                    postagens.push_back(p);
                }
            }
        }
    }
    
    return postagens;
}

std::vector<Stories> HttpClient::ParseStories(const std::string& json) {
    std::vector<Stories> stories;
    
    if (json.empty()) {
        return stories;
    }
    
    size_t pos = 0;
    int brace_count = 0;
    size_t obj_start = 0;
    
    for (size_t i = 0; i < json.length(); i++) {
        if (json[i] == '{') {
            if (brace_count == 0) {
                obj_start = i;
            }
            brace_count++;
        } else if (json[i] == '}') {
            brace_count--;
            if (brace_count == 0) {
                std::string obj = json.substr(obj_start, i - obj_start + 1);
                
                Stories s;
                s.idStories = ExtractJsonIntValue(obj, "idStories");
                if (s.idStories == 0) {
                    s.idStories = ExtractJsonIntValue(obj, "id_Stories");
                }
                s.conteudo = ExtractJsonStringValue(obj, "conteudo");
                
                int usuarioId = ExtractJsonIntValue(obj, "idUsuario");
                if (usuarioId == 0) {
                    usuarioId = ExtractJsonIntValue(obj, "id_usuario");
                }
                if (usuarioId == 0) {
                    size_t usuarioPos = obj.find("\"usuario\":");
                    if (usuarioPos != std::string::npos) {
                        size_t idPos = obj.find("\"idUsuario\":", usuarioPos);
                        if (idPos == std::string::npos) {
                            idPos = obj.find("\"id_usuario\":", usuarioPos);
                        }
                        if (idPos != std::string::npos) {
                            idPos += (obj.find("\"idUsuario\":", usuarioPos) != std::string::npos) ? 13 : 14;
                            size_t idEnd = obj.find_first_of(",}", idPos);
                            if (idEnd != std::string::npos) {
                                usuarioId = std::stoi(obj.substr(idPos, idEnd - idPos));
                            }
                        }
                    }
                }
                s.usuario_id = usuarioId;
                
                s.usuario_nome = ExtractJsonStringValue(obj, "usuario_nome");
                
                if (s.idStories > 0) {
                    stories.push_back(s);
                }
            }
        }
    }
    
    return stories;
}

std::vector<Curtida> HttpClient::ParseCurtidas(const std::string& json) {
    std::vector<Curtida> curtidas;
    
    if (json.empty()) {
        return curtidas;
    }
    
    size_t pos = 0;
    int brace_count = 0;
    size_t obj_start = 0;
    
    for (size_t i = 0; i < json.length(); i++) {
        if (json[i] == '{') {
            if (brace_count == 0) {
                obj_start = i;
            }
            brace_count++;
        } else if (json[i] == '}') {
            brace_count--;
            if (brace_count == 0) {
                std::string obj = json.substr(obj_start, i - obj_start + 1);
                
                Curtida c;
                c.id_curtida = ExtractJsonIntValue(obj, "id_curtida");
                if (c.id_curtida == 0) {
                    long long id = ExtractJsonIntValue(obj, "idCurtida");
                    if (id == 0) {
                        id = ExtractJsonIntValue(obj, "id");
                    }
                    c.id_curtida = id;
                }
                
                std::string dataStr = ExtractJsonStringValue(obj, "data_curtida");
                if (dataStr.empty()) {
                    dataStr = ExtractJsonStringValue(obj, "dataCurtida");
                }
                c.data_curtida = dataStr;
                
                int usuarioId = ExtractJsonIntValue(obj, "idUsuario");
                if (usuarioId == 0) {
                    usuarioId = ExtractJsonIntValue(obj, "id_usuario");
                }
                if (usuarioId == 0) {
                    size_t usuarioPos = obj.find("\"usuario\":");
                    if (usuarioPos != std::string::npos) {
                        size_t idPos = obj.find("\"idUsuario\":", usuarioPos);
                        if (idPos == std::string::npos) {
                            idPos = obj.find("\"id_usuario\":", usuarioPos);
                        }
                        if (idPos != std::string::npos) {
                            idPos += (obj.find("\"idUsuario\":", usuarioPos) != std::string::npos) ? 13 : 14;
                            size_t idEnd = obj.find_first_of(",}", idPos);
                            if (idEnd != std::string::npos) {
                                usuarioId = std::stoi(obj.substr(idPos, idEnd - idPos));
                            }
                        }
                    }
                }
                c.usuario_id = usuarioId;
                
                long postagemId = ExtractJsonIntValue(obj, "idPostagem");
                if (postagemId == 0) {
                    postagemId = ExtractJsonIntValue(obj, "id_postagem");
                }
                if (postagemId == 0) {
                    size_t postagemPos = obj.find("\"postagem\":");
                    if (postagemPos != std::string::npos) {
                        size_t idPos = obj.find("\"idPostagem\":", postagemPos);
                        if (idPos == std::string::npos) {
                            idPos = obj.find("\"id_postagem\":", postagemPos);
                        }
                        if (idPos != std::string::npos) {
                            idPos += (obj.find("\"idPostagem\":", postagemPos) != std::string::npos) ? 14 : 15;
                            size_t idEnd = obj.find_first_of(",}", idPos);
                            if (idEnd != std::string::npos) {
                                postagemId = std::stoi(obj.substr(idPos, idEnd - idPos));
                            }
                        }
                    }
                }
                c.postagem_id = postagemId;
                
                c.usuario_nome = ExtractJsonStringValue(obj, "usuario_nome");
                
                if (c.id_curtida > 0) {
                    curtidas.push_back(c);
                }
            }
        }
    }
    
    return curtidas;
}

std::vector<Comentario> HttpClient::ParseComentarios(const std::string& json) {
    std::vector<Comentario> comentarios;
    
    if (json.empty()) {
        return comentarios;
    }
    
    size_t pos = 0;
    int brace_count = 0;
    size_t obj_start = 0;
    
    for (size_t i = 0; i < json.length(); i++) {
        if (json[i] == '{') {
            if (brace_count == 0) {
                obj_start = i;
            }
            brace_count++;
        } else if (json[i] == '}') {
            brace_count--;
            if (brace_count == 0) {
                std::string obj = json.substr(obj_start, i - obj_start + 1);
                
                Comentario c;
                c.id_comentario = ExtractJsonIntValue(obj, "idComentario");
                if (c.id_comentario == 0) {
                    c.id_comentario = ExtractJsonIntValue(obj, "id_comentario");
                }
                c.conteudo = ExtractJsonStringValue(obj, "conteudo");
                
                std::string dataStr = ExtractJsonStringValue(obj, "dataComentario");
                if (dataStr.empty()) {
                    dataStr = ExtractJsonStringValue(obj, "data_comentario");
                }
                c.data_comentario = dataStr;
                
                int usuarioId = ExtractJsonIntValue(obj, "idUsuario");
                if (usuarioId == 0) {
                    usuarioId = ExtractJsonIntValue(obj, "id_usuario");
                }
                if (usuarioId == 0) {
                    size_t usuarioPos = obj.find("\"usuario\":");
                    if (usuarioPos != std::string::npos) {
                        size_t idPos = obj.find("\"idUsuario\":", usuarioPos);
                        if (idPos == std::string::npos) {
                            idPos = obj.find("\"id_usuario\":", usuarioPos);
                        }
                        if (idPos != std::string::npos) {
                            idPos += (obj.find("\"idUsuario\":", usuarioPos) != std::string::npos) ? 13 : 14;
                            size_t idEnd = obj.find_first_of(",}", idPos);
                            if (idEnd != std::string::npos) {
                                usuarioId = std::stoi(obj.substr(idPos, idEnd - idPos));
                            }
                        }
                    }
                }
                c.usuario_id = usuarioId;
                
                long postagemId = ExtractJsonIntValue(obj, "idPostagem");
                if (postagemId == 0) {
                    postagemId = ExtractJsonIntValue(obj, "id_postagem");
                }
                if (postagemId == 0) {
                    size_t postagemPos = obj.find("\"postagem\":");
                    if (postagemPos != std::string::npos) {
                        size_t idPos = obj.find("\"idPostagem\":", postagemPos);
                        if (idPos == std::string::npos) {
                            idPos = obj.find("\"id_postagem\":", postagemPos);
                        }
                        if (idPos != std::string::npos) {
                            idPos += (obj.find("\"idPostagem\":", postagemPos) != std::string::npos) ? 14 : 15;
                            size_t idEnd = obj.find_first_of(",}", idPos);
                            if (idEnd != std::string::npos) {
                                postagemId = std::stoi(obj.substr(idPos, idEnd - idPos));
                            }
                        }
                    }
                }
                c.postagem_id = postagemId;
                
                c.usuario_nome = ExtractJsonStringValue(obj, "usuario_nome");
                
                if (c.id_comentario > 0) {
                    comentarios.push_back(c);
                }
            }
        }
    }
    
    return comentarios;
}
