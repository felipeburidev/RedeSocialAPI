
INSERT INTO Usuario (nome, email, senha, data_nasc) VALUES
('Ana Silva', 'ana.silva@email.com', 'senha_hash_123', '1990-05-15'),
('Bruno Costa', 'bruno.costa@email.com', 'senha_hash_123', '1992-08-20'),
('Carla Dias', 'carla.dias@email.com', 'senha_hash_123', '1995-02-10'),
('Daniel Moreira', 'daniel.m@email.com', 'senha_hash_123', '1988-11-30'),
('Elisa Fernandes', 'elisa.f@email.com', 'senha_hash_123', '2000-07-25'),
('Felipe Guedes', 'felipe.guedes@email.com', 'senha_hash_123', '1993-01-05'),
('Gabriela Lima', 'gabriela.l@email.com', 'senha_hash_123', '1998-04-12'),
('Heitor Ramos', 'heitor.r@email.com', 'senha_hash_123', '1991-09-18'),
('Isis Oliveira', 'isis.o@email.com', 'senha_hash_123', '1997-06-08'),
('Joao Mendes', 'joao.mendes@email.com', 'senha_hash_123', '1994-03-22');

INSERT INTO Hashtag (nome) VALUES
('tech'), ('viagem'), ('programacao'), ('sqlite'), ('social'), ('culinaria');

INSERT INTO Perfil (id_usuario, bio, interesses) VALUES
(1, 'Engenheira de Software na Empresa X.', 'Livros, café, tech'),
(2, 'Fotógrafo explorando o mundo.', 'Viagem, trilhas, natureza'),
(3, 'Estudante de Design Gráfico.', 'Arte, música, filmes'),
(4, 'Desenvolvedor Backend Sênior.', 'Python, Go, bancos de dados'),
(5, 'Chef de cozinha e entusiasta de vinhos.', 'Gastronomia, vinhos, culinaria'),
(6, 'Músico e compositor.', 'Violão, rock, shows'),
(7, 'Jornalista investigativa.', 'Política, escrita, social'),
(8, 'Arquiteto focado em sustentabilidade.', 'Design urbano, ecologia'),
(9, 'Influencer digital.', 'Moda, maquiagem, viagem'),
(10, 'Atleta de fim de semana.', 'Corrida, musculação, esportes');

INSERT INTO Amizade (id_usuario_1, id_usuario_2, status) VALUES
(1, 2, 'aceito'),  -- Ana e Bruno
(1, 3, 'aceito'),  -- Ana e Carla
(2, 4, 'pendente'),-- Bruno e Daniel
(5, 6, 'aceito'),  -- Elisa e Felipe
(7, 8, 'aceito');  -- Gabriela e Heitor

INSERT INTO Stories (id_usuario, conteudo) VALUES
(1, 'url_story_ana_trabalho.jpg'),
(9, 'url_story_isis_look.mp4'),
(9, 'url_story_isis_make.jpg'),
(2, 'url_story_bruno_viagem.jpg');

INSERT INTO Notificacao (id_usuario, conteudo, status_leitura) VALUES
(1, 'Bruno Costa curtiu sua postagem.', false),
(4, 'Bruno Costa enviou uma solicitação de amizade.', false),
(1, 'Carla Dias comentou na sua postagem.', true),
(5, 'Felipe Guedes te convidou para o grupo "Amantes da Gastronomia".', false);

INSERT INTO Recompensa (id_usuario, criterio) VALUES
(7, 'Badge de Top Escritor - 10 artigos publicados'),
(4, 'Badge de Colaborador - 100 respostas no fórum');

INSERT INTO Denuncia (id_usuario_denunciante, id_usuario_denunciado, motivo, status, conteudo) VALUES
(8, 10, 'Este usuário está enviando spam em comentários.', 'pendente', 'Referente ao Post ID 4');

INSERT INTO Conversa (id_criador, nome, tipo) VALUES
(1, 'Chat com Bruno', 'individual'), -- Conversa 1
(4, 'Grupo Devs BR', 'grupo');      -- Conversa 2

INSERT INTO Postagem (id_usuario, conteudo, tipo) VALUES
(1, 'Meu primeiro post! Animada para me conectar. #tech', 'texto'),
(2, 'Foto incrível da trilha de ontem. #viagem', 'imagem'),
(4, 'Discutindo os prós e contras do SQLite em produção. O que acham? #programacao #sqlite', 'texto'),
(9, 'Look do dia! #social', 'imagem'),
(5, 'Receita nova no blog: Risoto de cogumelos. #culinaria', 'texto');

INSERT INTO Evento (id_criador, nome, local, data_evento) VALUES
(3, 'Exposição de Arte Digital', 'Galeria Central', '2025-11-20 19:00:00'),
(4, 'Meetup de Programação Python/Go', 'Hub de Inovação', '2025-11-25 18:30:00');

INSERT INTO Bloqueio (id_usuario_bloqueador, id_usuario_bloqueado, motivo) VALUES
(10, 8, 'Me denunciou sem motivo.');

INSERT INTO Grupo (id_administrador, nome, descricao, privacidade) VALUES
(4, 'Grupo Devs BR', 'Para discutir programação em geral.', 'publico'),
(5, 'Amantes da Gastronomia', 'Receitas e dicas de restaurantes.', 'privado');

INSERT INTO Mensagem (id_conversa, id_remetente, id_destinatario, conteudo) VALUES
(1, 1, 2, 'Oi Bruno, tudo bem? Vi suas fotos da trilha!'),
(1, 2, 1, 'Oi Ana! Tudo ótimo, e com você? Foi incrível!'),
(2, 4, NULL, 'Bem-vindos ao grupo de Devs! Sintam-se à vontade para compartilhar.'),
(2, 6, NULL, 'Opa, valeu pelo convite Daniel! Já chego com uma dúvida...');

-- 15. Ligar Postagens e Hashtags (M:N)
-- (Assumindo que os Posts são 1, 2, 3, 4, 5 e Hashtags 1-6)
INSERT INTO Postagem_Hashtag (id_postagem, id_hashtag) VALUES
(1, 1), -- Post 1 (Ana) -> #tech
(2, 2), -- Post 2 (Bruno) -> #viagem
(3, 3), -- Post 3 (Daniel) -> #programacao
(3, 4), -- Post 3 (Daniel) -> #sqlite
(4, 5), -- Post 4 (Isis) -> #social
(5, 6); -- Post 5 (Elisa) -> #culinaria

-- 16. Inserir Comentários (em posts)
INSERT INTO Comentario (id_postagem, id_usuario, conteudo) VALUES
(1, 3, 'Parabéns pelo primeiro post, Ana!'),
(1, 2, 'Show! Bem-vinda!'),
(2, 4, 'Que lugar lindo, Bruno! Onde é?'),
(3, 1, 'Ótimo tópico, Daniel! Usamos SQLite para apps mobile, mas nunca em produção web.'),
(5, 6, 'Parece delicioso, Elisa!');

-- 17. Inserir Curtidas (em posts)
INSERT INTO Curtida (id_usuario, id_postagem) VALUES
(2, 1), -- Bruno curtiu o post 1 (Ana)
(1, 2), -- Ana curtiu o post 2 (Bruno)
(3, 2), -- Carla curtiu o post 2 (Bruno)
(9, 3), -- Isis curtiu o post 3 (Daniel)
(1, 3), -- Ana curtiu o post 3 (Daniel)
(6, 5); -- Felipe curtiu o post 5 (Elisa)

-- 18. Inserir Participantes em Eventos (M:N)
INSERT INTO Usuario_Participa_Evento (id_usuario, id_evento) VALUES
(1, 1), -- Ana vai na Exposição de Arte
(3, 1), -- Carla (criadora) vai na Exposição de Arte
(4, 2), -- Daniel (criador) vai no Meetup
(1, 2), -- Ana também vai no Meetup
(6, 2), -- Felipe vai no Meetup
(8, 2); -- Heitor vai no Meetup

INSERT INTO MembroGrupo (id_usuario, id_grupo, funcao) VALUES
(4, 1, 'admin'),  -- Daniel (criador) é admin do Grupo 1
(5, 2, 'admin'),  -- Elisa (criadora) é admin do Grupo 2
(1, 1, 'membro'), -- Ana entra no Grupo 1
(6, 1, 'membro'), -- Felipe entra no Grupo 1
(6, 2, 'membro'), -- Felipe entra no Grupo 2
(10, 2, 'membro');-- Joao entra no Grupo 2