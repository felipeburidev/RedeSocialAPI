-- Script de População do Banco de Dados - Rede Social API
-- Requisito: Pelo menos 25 registros por entidade.
-- Ordem de inserção respeita as Foreign Keys (Usuario -> Perfil -> Outros).

-- 1. Inserir Usuários (25 registros)
INSERT INTO Usuario (nome, email, senha, data_nasc) VALUES
('Ana Silva', 'ana.silva@email.com', 'senha123', '1990-05-15'),
('Bruno Costa', 'bruno.costa@email.com', 'senha123', '1992-08-20'),
('Carla Dias', 'carla.dias@email.com', 'senha123', '1995-02-10'),
('Daniel Moreira', 'daniel.m@email.com', 'senha123', '1988-11-30'),
('Elisa Fernandes', 'elisa.f@email.com', 'senha123', '2000-07-25'),
('Felipe Guedes', 'felipe.guedes@email.com', 'senha123', '1993-01-05'),
('Gabriela Lima', 'gabriela.l@email.com', 'senha123', '1998-04-12'),
('Heitor Ramos', 'heitor.r@email.com', 'senha123', '1991-09-18'),
('Isis Oliveira', 'isis.o@email.com', 'senha123', '1997-06-08'),
('Joao Mendes', 'joao.mendes@email.com', 'senha123', '1994-03-22'),
('Lucas Pereira', 'lucas.p@email.com', 'senha123', '1999-12-01'),
('Mariana Souza', 'mariana.s@email.com', 'senha123', '1996-10-10'),
('Nicolas Cage', 'nicolas.c@email.com', 'senha123', '1985-01-07'),
('Olivia Palito', 'olivia.p@email.com', 'senha123', '1992-05-05'),
('Pedro Pascal', 'pedro.p@email.com', 'senha123', '1989-08-08'),
('Quintino Bocaiuva', 'quintino.b@email.com', 'senha123', '1990-02-02'),
('Rafael Nadal', 'rafael.n@email.com', 'senha123', '1986-06-03'),
('Sabrina Sato', 'sabrina.s@email.com', 'senha123', '1981-02-04'),
('Tatiana Maslany', 'tatiana.m@email.com', 'senha123', '1995-09-22'),
('Ursula Andress', 'ursula.a@email.com', 'senha123', '1990-01-01'),
('Vinicius Junior', 'vinicius.j@email.com', 'senha123', '2000-07-12'),
('Wagner Moura', 'wagner.m@email.com', 'senha123', '1976-06-27'),
('Xuxa Meneghel', 'xuxa.m@email.com', 'senha123', '1963-03-27'),
('Yasmin Brunet', 'yasmin.b@email.com', 'senha123', '1988-06-06'),
('Zeca Pagodinho', 'zeca.p@email.com', 'senha123', '1959-02-04');

-- 2. Inserir Perfis (25 registros - 1 para cada usuário)
INSERT INTO Perfil (id_usuario, privacidade, foto, bio, interesses) VALUES
(1, 'publico', 'foto1.jpg', 'Engenheira de Software', 'Tecnologia, Leitura'),
(2, 'publico', 'foto2.jpg', 'Fotógrafo', 'Viagem, Natureza'),
(3, 'privado', 'foto3.jpg', 'Designer', 'Arte, Cinema'),
(4, 'publico', 'foto4.jpg', 'Desenvolvedor', 'Código, Café'),
(5, 'publico', 'foto5.jpg', 'Chef', 'Culinária, Vinhos'),
(6, 'privado', 'foto6.jpg', 'Músico', 'Rock, Jazz'),
(7, 'publico', 'foto7.jpg', 'Jornalista', 'Notícias, Política'),
(8, 'publico', 'foto8.jpg', 'Arquiteto', 'Construção, Design'),
(9, 'publico', 'foto9.jpg', 'Influencer', 'Moda, Lifestyle'),
(10, 'privado', 'foto10.jpg', 'Atleta', 'Esportes, Saúde'),
(11, 'publico', 'foto11.jpg', 'Estudante', 'Games, Anime'),
(12, 'publico', 'foto12.jpg', 'Advogada', 'Direito, Leis'),
(13, 'privado', 'foto13.jpg', 'Ator', 'Cinema, Teatro'),
(14, 'publico', 'foto14.jpg', 'Modelo', 'Moda, Passarela'),
(15, 'publico', 'foto15.jpg', 'Diretor', 'Filmes, Séries'),
(16, 'privado', 'foto16.jpg', 'Historiador', 'História, Livros'),
(17, 'publico', 'foto17.jpg', 'Tenista', 'Tênis, Esporte'),
(18, 'publico', 'foto18.jpg', 'Apresentadora', 'TV, Entretenimento'),
(19, 'privado', 'foto19.jpg', 'Atriz', 'Séries, Sci-fi'),
(20, 'publico', 'foto20.jpg', 'Veterinária', 'Animais, Natureza'),
(21, 'publico', 'foto21.jpg', 'Jogador', 'Futebol, Real Madrid'),
(22, 'publico', 'foto22.jpg', 'Diretor', 'Cinema Brasileiro'),
(23, 'publico', 'foto23.jpg', 'Apresentadora', 'Crianças, TV'),
(24, 'privado', 'foto24.jpg', 'Modelo', 'Moda, Praia'),
(25, 'publico', 'foto25.jpg', 'Cantor', 'Samba, Cerveja');

-- 3. Inserir Amizades (25 registros variados)
INSERT INTO Amizade (id_usuario_1, id_usuario_2, status) VALUES
(1, 2, 'aceito'), (1, 3, 'aceito'), (2, 4, 'pendente'), (5, 6, 'aceito'), (7, 8, 'aceito'),
(9, 10, 'pendente'), (11, 12, 'aceito'), (13, 14, 'aceito'), (15, 16, 'bloqueado'), (17, 18, 'aceito'),
(19, 20, 'aceito'), (21, 22, 'aceito'), (23, 24, 'pendente'), (25, 1, 'aceito'), (2, 5, 'aceito'),
(3, 6, 'aceito'), (4, 7, 'pendente'), (8, 9, 'aceito'), (10, 11, 'aceito'), (12, 13, 'aceito'),
(14, 15, 'aceito'), (16, 17, 'pendente'), (18, 19, 'aceito'), (20, 21, 'aceito'), (22, 23, 'aceito');

-- 4. Inserir Hashtags (25 registros)
INSERT INTO Hashtag (nome) VALUES
('tech'), ('viagem'), ('programacao'), ('sqlite'), ('social'), ('culinaria'), ('fitness'), ('musica'),
('cinema'), ('arte'), ('natureza'), ('futebol'), ('moda'), ('politica'), ('educacao'), ('saude'),
('marketing'), ('design'), ('arquitetura'), ('games'), ('leitura'), ('historia'), ('ciencia'), ('humor'), ('amor');

-- 5. Inserir Postagens (25 registros)
INSERT INTO Postagem (id_usuario, conteudo, tipo, data_publicacao) VALUES
(1, 'Meu primeiro post! #tech', 'texto', '2025-01-01 10:00:00'),
(2, 'Foto da viagem #viagem', 'imagem', '2025-01-02 11:00:00'),
(3, 'Novo desenho #arte', 'imagem', '2025-01-03 12:00:00'),
(4, 'Codando em Java #programacao', 'texto', '2025-01-04 13:00:00'),
(5, 'Receita de Risoto #culinaria', 'texto', '2025-01-05 14:00:00'),
(6, 'Tocando guitarra #musica', 'video', '2025-01-06 15:00:00'),
(7, 'Notícia urgente #politica', 'texto', '2025-01-07 16:00:00'),
(8, 'Projeto da casa nova #arquitetura', 'imagem', '2025-01-08 17:00:00'),
(9, 'Look do dia #moda', 'imagem', '2025-01-09 18:00:00'),
(10, 'Treino pesado #fitness', 'video', '2025-01-10 19:00:00'),
(11, 'Zerando o jogo #games', 'imagem', '2025-01-11 20:00:00'),
(12, 'Estudando para o exame #educacao', 'texto', '2025-01-12 21:00:00'),
(13, 'Bastidores da gravação #cinema', 'video', '2025-01-13 22:00:00'),
(14, 'Desfile em Paris #moda', 'imagem', '2025-01-14 08:00:00'),
(15, 'Roteiro finalizado #cinema', 'texto', '2025-01-15 09:00:00'),
(16, 'Livro interessante #leitura', 'imagem', '2025-01-16 10:00:00'),
(17, 'Venci o campeonato! #futebol', 'imagem', '2025-01-17 11:00:00'),
(18, 'Ao vivo na TV #social', 'video', '2025-01-18 12:00:00'),
(19, 'Nova série chegando #cinema', 'imagem', '2025-01-19 13:00:00'),
(20, 'Cuidando dos bichinhos #natureza', 'imagem', '2025-01-20 14:00:00'),
(21, 'Gol da vitória! #futebol', 'video', '2025-01-21 15:00:00'),
(22, 'Filme novo em breve #cinema', 'texto', '2025-01-22 16:00:00'),
(23, 'Programa especial hoje #tv', 'imagem', '2025-01-23 17:00:00'),
(24, 'Sol e mar #viagem', 'imagem', '2025-01-24 18:00:00'),
(25, 'Samba no pé #musica', 'video', '2025-01-25 19:00:00');

-- 6. Inserir Postagem_Hashtag (25 relações)
INSERT INTO Postagem_Hashtag (id_postagem, id_hashtag) VALUES
(1, 1), (2, 2), (3, 10), (4, 3), (5, 6), (6, 8), (7, 14), (8, 19), (9, 13), (10, 7),
(11, 20), (12, 15), (13, 9), (14, 13), (15, 9), (16, 21), (17, 12), (18, 5), (19, 9), (20, 11),
(21, 12), (22, 9), (23, 5), (24, 2), (25, 8);

-- 7. Inserir Stories (25 registros)
INSERT INTO Stories (id_usuario, conteudo, data_criacao) VALUES
(1, 'story1.jpg', NOW()), (2, 'story2.jpg', NOW()), (3, 'story3.jpg', NOW()), (4, 'story4.jpg', NOW()), (5, 'story5.jpg', NOW()),
(6, 'story6.jpg', NOW()), (7, 'story7.jpg', NOW()), (8, 'story8.jpg', NOW()), (9, 'story9.jpg', NOW()), (10, 'story10.jpg', NOW()),
(11, 'story11.jpg', NOW()), (12, 'story12.jpg', NOW()), (13, 'story13.jpg', NOW()), (14, 'story14.jpg', NOW()), (15, 'story15.jpg', NOW()),
(16, 'story16.jpg', NOW()), (17, 'story17.jpg', NOW()), (18, 'story18.jpg', NOW()), (19, 'story19.jpg', NOW()), (20, 'story20.jpg', NOW()),
(21, 'story21.jpg', NOW()), (22, 'story22.jpg', NOW()), (23, 'story23.jpg', NOW()), (24, 'story24.jpg', NOW()), (25, 'story25.jpg', NOW());

-- 8. Inserir Comentários (25 registros)
INSERT INTO Comentario (id_postagem, id_usuario, conteudo) VALUES
(1, 2, 'Legal!'), (1, 3, 'Parabéns'), (2, 4, 'Onde é?'), (2, 5, 'Lindo'), (3, 6, 'Uau'),
(4, 7, 'Java é top'), (5, 8, 'Delícia'), (6, 9, 'Toca muito'), (7, 10, 'Triste'), (8, 11, 'Top'),
(9, 12, 'Arrasou'), (10, 13, 'Força'), (11, 14, 'Qual jogo?'), (12, 15, 'Boa sorte'), (13, 16, 'Ansioso'),
(14, 17, 'Chique'), (15, 18, 'Sucesso'), (16, 19, 'Li também'), (17, 20, 'Craque'), (18, 21, 'Vi ao vivo'),
(19, 22, 'Quero ver'), (20, 23, 'Fofos'), (21, 24, 'Golaco'), (22, 25, 'Vou assistir'), (23, 1, 'Adoro');

-- 9. Inserir Curtidas (25 registros)
INSERT INTO Curtida (id_usuario, id_postagem) VALUES
(2, 1), (3, 1), (4, 2), (5, 2), (6, 3), (7, 4), (8, 5), (9, 6), (10, 7), (11, 8),
(12, 9), (13, 10), (14, 11), (15, 12), (16, 13), (17, 14), (18, 15), (19, 16), (20, 17), (21, 18),
(22, 19), (23, 20), (24, 21), (25, 22), (1, 23);

-- 10. Inserir Grupos (25 registros)
INSERT INTO Grupo (id_administrador, nome, descricao, privacidade) VALUES
(1, 'Grupo Tech', 'Discussão sobre TI', 'publico'),
(2, 'Amantes de Foto', 'Dicas de fotografia', 'publico'),
(3, 'Cinefilos', 'Filmes e séries', 'privado'),
(4, 'Java Devs', 'Spring Boot', 'publico'),
(5, 'Receitas Fáceis', 'Culinária rápida', 'publico'),
(6, 'Músicos BR', 'Para quem toca', 'publico'),
(7, 'Jornalismo Hoje', 'Notícias', 'privado'),
(8, 'Arquitetura Moderna', 'Projetos', 'publico'),
(9, 'Fashion Week', 'Moda', 'privado'),
(10, 'Vida Saudável', 'Fitness', 'publico'),
(11, 'Gamers Unidos', 'Jogos online', 'publico'),
(12, 'OAB Estudos', 'Direito', 'privado'),
(13, 'Teatro Amador', 'Atores', 'publico'),
(14, 'Modelos BR', 'Carreira', 'privado'),
(15, 'Roteiristas', 'Escrita', 'publico'),
(16, 'História Geral', 'Estudos', 'publico'),
(17, 'Tênis Clube', 'Esporte', 'publico'),
(18, 'TV Bastidores', 'Mídia', 'privado'),
(19, 'Fãs de Sci-fi', 'Ficção', 'publico'),
(20, 'Mundo Pet', 'Animais', 'publico'),
(21, 'Futebol Arte', 'Resenha', 'publico'),
(22, 'Cinema Nacional', 'Apoio', 'publico'),
(23, 'Mães e Filhos', 'Dicas', 'publico'),
(24, 'Verão o Ano Todo', 'Praia', 'publico'),
(25, 'Roda de Samba', 'Música', 'publico');

-- 11. Inserir MembroGrupo (25 registros)
INSERT INTO MembroGrupo (id_usuario, id_grupo, funcao) VALUES
(2, 1, 'membro'), (3, 1, 'membro'), (4, 2, 'membro'), (5, 2, 'membro'), (6, 3, 'membro'),
(7, 4, 'membro'), (8, 5, 'membro'), (9, 6, 'membro'), (10, 7, 'membro'), (11, 8, 'membro'),
(12, 9, 'membro'), (13, 10, 'membro'), (14, 11, 'membro'), (15, 12, 'membro'), (16, 13, 'membro'),
(17, 14, 'membro'), (18, 15, 'membro'), (19, 16, 'membro'), (20, 17, 'membro'), (21, 18, 'membro'),
(22, 19, 'membro'), (23, 20, 'membro'), (24, 21, 'membro'), (25, 22, 'membro'), (1, 23, 'membro');

-- 12. Inserir Eventos (25 registros)
INSERT INTO Evento (id_criador, nome, local, data_evento) VALUES
(1, 'Hackathon', 'Online', '2025-12-01 10:00:00'),
(2, 'Workshop Foto', 'Estúdio A', '2025-12-02 14:00:00'),
(3, 'Maratona Filmes', 'Cinema B', '2025-12-03 18:00:00'),
(4, 'Java Meetup', 'Auditório', '2025-12-04 19:00:00'),
(5, 'Jantar Beneficente', 'Restaurante C', '2025-12-05 20:00:00'),
(6, 'Show de Talentos', 'Teatro D', '2025-12-06 15:00:00'),
(7, 'Debate Político', 'Universidade', '2025-12-07 10:00:00'),
(8, 'Feira de Design', 'Centro de Convenções', '2025-12-08 09:00:00'),
(9, 'Bazar Fashion', 'Shopping E', '2025-12-09 11:00:00'),
(10, 'Corrida 5k', 'Parque F', '2025-12-10 07:00:00'),
(11, 'Torneio LoL', 'Cyber Café', '2025-12-11 13:00:00'),
(12, 'Palestra Direito', 'Fórum', '2025-12-12 19:00:00'),
(13, 'Peça de Teatro', 'Teatro G', '2025-12-13 20:00:00'),
(14, 'Desfile Verão', 'Praia H', '2025-12-14 16:00:00'),
(15, 'Masterclass Roteiro', 'Escola I', '2025-12-15 14:00:00'),
(16, 'Exposição História', 'Museu J', '2025-12-16 10:00:00'),
(17, 'Open de Tênis', 'Clube K', '2025-12-17 08:00:00'),
(18, 'Visita Estúdio TV', 'Emissora L', '2025-12-18 14:00:00'),
(19, 'Convenção Sci-fi', 'Hotel M', '2025-12-19 10:00:00'),
(20, 'Feira de Adoção', 'Praça N', '2025-12-20 09:00:00'),
(21, 'Jogo Beneficente', 'Estádio O', '2025-12-21 16:00:00'),
(22, 'Mostra de Cinema', 'Cine P', '2025-12-22 18:00:00'),
(23, 'Encontro de Mães', 'Parque Q', '2025-12-23 15:00:00'),
(24, 'Luau na Praia', 'Praia R', '2025-12-24 20:00:00'),
(25, 'Samba na Praça', 'Praça S', '2025-12-25 17:00:00');

-- 13. Inserir Usuario_Participa_Evento (25 registros)
INSERT INTO Usuario_Participa_Evento (id_usuario, id_evento) VALUES
(2, 1), (3, 1), (4, 2), (5, 2), (6, 3), (7, 4), (8, 5), (9, 6), (10, 7), (11, 8),
(12, 9), (13, 10), (14, 11), (15, 12), (16, 13), (17, 14), (18, 15), (19, 16), (20, 17), (21, 18),
(22, 19), (23, 20), (24, 21), (25, 22), (1, 23);

-- 14. Inserir Conversas (25 registros)
INSERT INTO Conversa (id_criador, nome, tipo) VALUES
(1, 'Chat Ana-Bruno', 'individual'), (2, 'Chat Bruno-Carla', 'individual'), (3, 'Chat Carla-Daniel', 'individual'), (4, 'Grupo Devs', 'grupo'), (5, 'Chat Elisa-Felipe', 'individual'),
(6, 'Chat Felipe-Gabriela', 'individual'), (7, 'Chat Gabriela-Heitor', 'individual'), (8, 'Grupo Arquitetos', 'grupo'), (9, 'Chat Isis-Joao', 'individual'), (10, 'Chat Joao-Lucas', 'individual'),
(11, 'Chat Lucas-Mariana', 'individual'), (12, 'Grupo OAB', 'grupo'), (13, 'Chat Nicolas-Olivia', 'individual'), (14, 'Chat Olivia-Pedro', 'individual'), (15, 'Chat Pedro-Quintino', 'individual'),
(16, 'Grupo Historia', 'grupo'), (17, 'Chat Rafael-Sabrina', 'individual'), (18, 'Chat Sabrina-Tatiana', 'individual'), (19, 'Chat Tatiana-Ursula', 'individual'), (20, 'Grupo Pets', 'grupo'),
(21, 'Chat Vinicius-Wagner', 'individual'), (22, 'Chat Wagner-Xuxa', 'individual'), (23, 'Chat Xuxa-Yasmin', 'individual'), (24, 'Grupo Praia', 'grupo'), (25, 'Chat Zeca-Ana', 'individual');

-- 15. Inserir Mensagens (25 registros)
INSERT INTO Mensagem (id_conversa, id_remetente, id_destinatario, conteudo, status) VALUES
(1, 1, 2, 'Oi Bruno', 'enviado'), (1, 2, 1, 'Oi Ana', 'lido'), (2, 2, 3, 'Oi Carla', 'enviado'), (4, 4, NULL, 'Ola grupo', 'lido'), (5, 5, 6, 'Oi Felipe', 'enviado'),
(6, 6, 7, 'Oi Gabi', 'lido'), (7, 7, 8, 'Oi Heitor', 'enviado'), (8, 8, NULL, 'Ola arquitetos', 'lido'), (9, 9, 10, 'Oi Joao', 'enviado'), (10, 10, 11, 'Oi Lucas', 'lido'),
(11, 11, 12, 'Oi Mari', 'enviado'), (12, 12, NULL, 'Ola advogados', 'lido'), (13, 13, 14, 'Oi Olivia', 'enviado'), (14, 14, 15, 'Oi Pedro', 'lido'), (15, 15, 16, 'Oi Quintino', 'enviado'),
(16, 16, NULL, 'Ola historiadores', 'lido'), (17, 17, 18, 'Oi Sa', 'enviado'), (18, 18, 19, 'Oi Tati', 'lido'), (19, 19, 20, 'Oi Ursula', 'enviado'), (20, 20, NULL, 'Ola pet lovers', 'lido'),
(21, 21, 22, 'Oi Wagner', 'enviado'), (22, 22, 23, 'Oi Xuxa', 'lido'), (23, 23, 24, 'Oi Yasmin', 'enviado'), (24, 24, NULL, 'Ola praieiros', 'lido'), (25, 25, 1, 'Oi Ana', 'enviado');

-- 16. Inserir Notificações (25 registros)
INSERT INTO Notificacao (id_usuario, conteudo) VALUES
(1, 'Nova curtida'), (2, 'Novo seguidor'), (3, 'Novo comentário'), (4, 'Novo grupo'), (5, 'Nova mensagem'),
(6, 'Evento próximo'), (7, 'Feliz aniversário'), (8, 'Sugestão de amigo'), (9, 'Nova foto'), (10, 'Atualização de sistema'),
(11, 'Nova curtida'), (12, 'Novo seguidor'), (13, 'Novo comentário'), (14, 'Novo grupo'), (15, 'Nova mensagem'),
(16, 'Evento próximo'), (17, 'Feliz aniversário'), (18, 'Sugestão de amigo'), (19, 'Nova foto'), (20, 'Atualização de sistema'),
(21, 'Nova curtida'), (22, 'Novo seguidor'), (23, 'Novo comentário'), (24, 'Novo grupo'), (25, 'Nova mensagem');

-- 17. Inserir Recompensas (25 registros)
INSERT INTO Recompensa (id_usuario, criterio) VALUES
(1, 'Top Fan'), (2, 'Influencer'), (3, 'Comentarista'), (4, 'Admin'), (5, 'Chef'),
(6, 'Músico'), (7, 'Jornalista'), (8, 'Arquiteto'), (9, 'Fashionista'), (10, 'Atleta'),
(11, 'Gamer'), (12, 'Advogado'), (13, 'Ator'), (14, 'Modelo'), (15, 'Roteirista'),
(16, 'Historiador'), (17, 'Tenista'), (18, 'Apresentador'), (19, 'Atriz'), (20, 'Pet Lover'),
(21, 'Jogador'), (22, 'Cineasta'), (23, 'Mãe'), (24, 'Praieiro'), (25, 'Sambista');

-- 18. Inserir Denúncias (25 registros)
INSERT INTO Denuncia (id_usuario_denunciante, id_usuario_denunciado, motivo, conteudo) VALUES
(1, 25, 'Spam', 'Mensagem repetida'), (2, 24, 'Ofensa', 'Comentário ofensivo'), (3, 23, 'Fake News', 'Post falso'), (4, 22, 'Spam', 'Link suspeito'), (5, 21, 'Ofensa', 'Xingamento'),
(6, 20, 'Spam', 'Venda ilegal'), (7, 19, 'Fake', 'Perfil falso'), (8, 18, 'Direitos Autorais', 'Foto roubada'), (9, 17, 'Spam', 'Bot'), (10, 16, 'Ofensa', 'Assédio'),
(11, 15, 'Spam', 'Propaganda'), (12, 14, 'Fake', 'Impostor'), (13, 13, 'Ofensa', 'Ameaça'), (14, 12, 'Spam', 'Corrente'), (15, 11, 'Cheat', 'Trapaça no jogo'),
(16, 10, 'Spam', 'Flood'), (17, 9, 'Fake', 'Foto fake'), (18, 8, 'Direitos Autorais', 'Projeto roubado'), (19, 7, 'Fake News', 'Mentira'), (20, 6, 'Spam', 'Divulgação excessiva'),
(21, 5, 'Ofensa', 'Crítica destrutiva'), (22, 4, 'Spam', 'Convite massivo'), (23, 3, 'Fake', 'Bot'), (24, 2, 'Ofensa', 'Bullying'), (25, 1, 'Spam', 'Email falso');

-- 19. Inserir Bloqueios (25 registros)
INSERT INTO Bloqueio (id_usuario_bloqueador, id_usuario_bloqueado, motivo) VALUES
(1, 25, 'Chato'), (2, 24, 'Brigamos'), (3, 23, 'Não gosto'), (4, 22, 'Spam'), (5, 21, 'Rival'),
(6, 20, 'Vendas'), (7, 19, 'Fake'), (8, 18, 'Cópia'), (9, 17, 'Bot'), (10, 16, 'Assédio'),
(11, 15, 'Chato'), (12, 14, 'Brigamos'), (13, 13, 'Não gosto'), (14, 12, 'Spam'), (15, 11, 'Rival'),
(16, 10, 'Vendas'), (17, 9, 'Fake'), (18, 8, 'Cópia'), (19, 7, 'Bot'), (20, 6, 'Assédio'),
(21, 5, 'Chato'), (22, 4, 'Brigamos'), (23, 3, 'Não gosto'), (24, 2, 'Spam'), (25, 1, 'Rival');