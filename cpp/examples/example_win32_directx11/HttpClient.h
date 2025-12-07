#pragma once

#define CURL_STATICLIB
#include "curl/curl.h"
#include <string>
#include <vector>

struct Usuario {
    int id_usuario = 0;
    std::string nome;
    std::string email;
    std::string senha;
    std::string dataNasc;
};

struct Postagem {
    long id_postagem = 0;
    std::string conteudo;
    std::string tipo;
    std::string data;
    int usuario_id = 0;
    std::string usuario_nome;
};

struct Stories {
    int idStories = 0;
    std::string conteudo;
    int usuario_id = 0;
    std::string usuario_nome;
};

struct Curtida {
    long id_curtida = 0;
    std::string data_curtida;
    int usuario_id = 0;
    std::string usuario_nome;
    long postagem_id = 0;
};

struct Comentario {
    int id_comentario = 0;
    std::string conteudo;
    std::string data_comentario;
    int usuario_id = 0;
    std::string usuario_nome;
    long postagem_id = 0;
};

class HttpClient {
public:
    HttpClient(const std::string& baseUrl = "http://localhost:9090");
    
    Usuario Login(const std::string& email, const std::string& senha);
    bool Register(const Usuario& usuario);
    
    std::string GetUsuarios();
    std::string GetUsuarioById(int id);
    std::string CreateUsuario(const Usuario& usuario);
    std::string UpdateUsuario(int id, const Usuario& usuario);
    std::string DeleteUsuario(int id);
    std::string SearchUsuarioByName(const std::string& nome);
    
    std::string GetPostagens();
    std::string GetPostagemById(int id);
    std::string QueryPostagens(int usuario_id = -1, const std::string& tipo = "");
    std::string CreatePostagem(const Postagem& postagem);
    std::string UpdatePostagem(int id, const Postagem& postagem);
    std::string DeletePostagem(int id);
    
    std::string GetStories();
    std::string GetStoryById(int id);
    std::string QueryStories(int usuario_id = -1);
    std::string CreateStory(const Stories& story);
    std::string UpdateStory(int id, const Stories& story);
    std::string DeleteStory(int id);
    
    std::string GetCurtidas();
    std::string GetCurtidaById(int id);
    std::string QueryCurtidas(int usuario_id = -1, int postagem_id = -1);
    std::string CreateCurtida(const Curtida& curtida);
    std::string UpdateCurtida(int id, const Curtida& curtida);
    std::string DeleteCurtida(int id);
    
    std::string GetComentarios();
    std::string GetComentarioById(int id);
    std::string QueryComentarios(int postagem_id = -1);
    std::string CreateComentario(int postagem_id, int usuario_id, const Comentario& comentario);
    std::string UpdateComentario(int id, const Comentario& comentario);
    std::string DeleteComentario(int id);
    
    std::string MakeRequest(const std::string& method, const std::string& endpoint, const std::string& body = "");
    
    std::vector<Usuario> ParseUsuarios(const std::string& json);
    std::vector<Postagem> ParsePostagens(const std::string& json);
    std::vector<Stories> ParseStories(const std::string& json);
    std::vector<Curtida> ParseCurtidas(const std::string& json);
    std::vector<Comentario> ParseComentarios(const std::string& json);
    
    std::string GetLastError() const { return lastError; }
    
private:
    std::string baseUrl;
    std::string lastError;
    
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s);
};

