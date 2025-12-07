# Projeto de modelagem de dados e estrutura de dados: Rede Social API

Este projeto é um trabalho em conjunto entre os integrantes José Vinicius, Gabriel Bispo, Jean Pierre e Felipe Buri.

# Modelagem de Dados

O banco de dados foi feito em SQl, utilizando o SGDB Postgres.

São 19 tabelas, sendo elas usuário, perfil, amizade, stories, notificação, recompensa, denúncia, conversa, mensagem, postagem, hashtag, postagem_hashtag, comentario, curtida, evento, usuario_participa_evento, bloqueio, grupo e membrogrupo.

# Estrutura de Dados

A API foi construida em Java, com spring boot utilizando Maven por conta de seu desempenho e robustez.

# Rotas da API

1. Usuários (UsuarioController)
Base Path: /usuarios

Listar todos (GET): http://localhost:8080/usuarios/get

Buscar por ID (GET): http://localhost:8080/usuarios/id/1

Buscar por Nome (GET): http://localhost:8080/usuarios?nome=Maria

Criar Usuário (POST): URL: http://localhost:8080/usuarios/post Body (JSON): (Baseado na classe Usuario.java)

JSON
``
{
  "nome": "João Silva",
  "email": "joao@email.com",
  "senha": "123",
  "dataNasc": "1990-01-01"
}``

Deletar (DELETE): Nota: A rota no código está definida como /dell{id}. http://localhost:8080/usuarios/dell1

Atualizar (PUT): http://localhost:8080/usuarios/alter1

2. Postagens (PostagemController)
Base Path: /posts

Listar todas (GET): http://localhost:8080/posts

Buscar por ID (GET): http://localhost:8080/posts/1

Criar Postagem (POST): Nota: A rota no código está definida como Post (com P maiúsculo). URL: http://localhost:8080/posts/Post Body (JSON): (Baseado na classe Postagem.java)

JSON
``
{
  "conteudo": "Olá mundo, este é meu post!",
  "tipo": "Texto",
  "data": "2025-11-24",
  "usuario": {
    "id_usuario": 1
  }
}``

Deletar (DELETE): Nota: A rota é delete{id}. http://localhost:8080/posts/delete1

3. Stories (StoriesController)
Base Path: /Stories (Atenção: O código usa 'Stories' com S maiúsculo na anotação @RequestMapping)

Listar todos (GET): http://localhost:8080/Stories/AllStories

Buscar por ID (GET): http://localhost:8080/Stories/1

Criar Story (POST): URL: http://localhost:8080/Stories/post Body (JSON): (Baseado na classe Stories.java)

JSON
``
{
  "conteudo": "Minha foto de férias",
  "usuario": {
    "id_usuario": 1
  }
}``

Deletar (DELETE): Nota: A rota é /DeletarStories{id}. http://localhost:8080/Stories/DeletarStories1

4. Curtidas (CurtidaController)
Base Path: /curtida (Atenção: singular e minúsculo, conforme o código)

Listar todas (GET): http://localhost:8080/curtida

Criar Curtida (POST): URL: http://localhost:8080/curtida Body (JSON): (Baseado na classe Curtida.java)

JSON
``
{
  "data_curtida": "2025-11-24",
  "usuario": {
    "id_usuario": 1
  },
  "postagem": {
    "id_postagem": 1
  }
}``

Deletar (DELETE): http://localhost:8080/curtida/delete1

# Para compilar e rodar

Linux/Mac: ./mvnw spring-boot:run

Windows: mvnw spring-boot:run
