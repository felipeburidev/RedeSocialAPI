-- Tabela principal de Usuários
CREATE TABLE Usuario (
    id_usuario SERIAL PRIMARY KEY,
    nome VARCHAR(100) NOT NULL,
    email VARCHAR(100) NOT NULL UNIQUE,
    senha VARCHAR(255) NOT NULL,
    data_nasc DATE
);

-- Tabela de Perfil (Relação 1:1 com Usuário)
CREATE TABLE Perfil (
    id_usuario INT PRIMARY KEY,
    privacidade VARCHAR(50),
    foto VARCHAR(255),
    bio TEXT,
    interesses TEXT,
    FOREIGN KEY (id_usuario) REFERENCES Usuario(id_usuario)
);

-- Tabela de Amizade (Relação M:N entre Usuários)
CREATE TABLE Amizade (
    id_usuario_1 INT,
    id_usuario_2 INT,
    data_inicio TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    status VARCHAR(50) DEFAULT 'pendente',
    PRIMARY KEY (id_usuario_1, id_usuario_2),
    FOREIGN KEY (id_usuario_1) REFERENCES Usuario(id_usuario),
    FOREIGN KEY (id_usuario_2) REFERENCES Usuario(id_usuario),
    CHECK (id_usuario_1 <> id_usuario_2)
);

-- Tabela de Stories (Relação 1:N com Usuário)
CREATE TABLE Stories (
    id_story SERIAL PRIMARY KEY,
    id_usuario INT NOT NULL,
    conteudo VARCHAR(255) NOT NULL,
    data_criacao TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (id_usuario) REFERENCES Usuario(id_usuario)
);

-- Tabela de Notificações (Relação 1:N com Usuário)
CREATE TABLE Notificacao (
    id_notificacao SERIAL PRIMARY KEY,
    id_usuario INT NOT NULL,
    conteudo TEXT NOT NULL,
    status_leitura BOOLEAN DEFAULT FALSE,
    data_criacao TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (id_usuario) REFERENCES Usuario(id_usuario)
);

-- Tabela de Recompensas (Relação 1:N com Usuário)
CREATE TABLE Recompensa (
    id_recompensa SERIAL PRIMARY KEY,
    id_usuario INT NOT NULL,
    criterio VARCHAR(255),
    data_recebimento TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (id_usuario) REFERENCES Usuario(id_usuario)
);

-- Tabela de Denúncias
CREATE TABLE Denuncia (
    id_denuncia SERIAL PRIMARY KEY,
    id_usuario_denunciante INT NOT NULL,
    id_usuario_denunciado INT NOT NULL,
    data TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    motivo TEXT NOT NULL,
    status VARCHAR(50) DEFAULT 'pendente',
    conteudo TEXT,
    FOREIGN KEY (id_usuario_denunciante) REFERENCES Usuario(id_usuario),
    FOREIGN KEY (id_usuario_denunciado) REFERENCES Usuario(id_usuario)
);

-- Tabela de Conversas
CREATE TABLE Conversa (
    id_conversa SERIAL PRIMARY KEY,
    id_criador INT NOT NULL,
    nome VARCHAR(100),
    data_criacao TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    tipo VARCHAR(50) NOT NULL, -- 'individual' ou 'grupo'
    FOREIGN KEY (id_criador) REFERENCES Usuario(id_usuario)
);

-- Tabela de Mensagens
CREATE TABLE Mensagem (
    id_mensagem SERIAL PRIMARY KEY,
    id_conversa INT NOT NULL,
    id_remetente INT NOT NULL,
    id_destinatario INT,
    conteudo TEXT NOT NULL,
    data_envio TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    status VARCHAR(50),
    FOREIGN KEY (id_conversa) REFERENCES Conversa(id_conversa),
    FOREIGN KEY (id_remetente) REFERENCES Usuario(id_usuario),
    FOREIGN KEY (id_destinatario) REFERENCES Usuario(id_usuario)
);

-- Tabela de Postagens
CREATE TABLE Postagem (
    id_postagem SERIAL PRIMARY KEY,
    id_usuario INT NOT NULL,
    conteudo TEXT,
    data_publicacao TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    tipo VARCHAR(50),
    FOREIGN KEY (id_usuario) REFERENCES Usuario(id_usuario)
);

-- Tabela de Hashtags
CREATE TABLE Hashtag (
    id_hashtag SERIAL PRIMARY KEY,
    nome VARCHAR(100) NOT NULL UNIQUE
);

-- Tabela de Junção Postagem_Hashtag (Relação M:N)
CREATE TABLE Postagem_Hashtag (
    id_postagem INT NOT NULL,
    id_hashtag INT NOT NULL,
    PRIMARY KEY (id_postagem, id_hashtag),
    FOREIGN KEY (id_postagem) REFERENCES Postagem(id_postagem),
    FOREIGN KEY (id_hashtag) REFERENCES Hashtag(id_hashtag)
);

-- Tabela de Comentários
CREATE TABLE Comentario (
    id_comentario SERIAL PRIMARY KEY,
    id_postagem INT NOT NULL,
    id_usuario INT NOT NULL,
    conteudo TEXT NOT NULL,
    data_comentario TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (id_postagem) REFERENCES Postagem(id_postagem),
    FOREIGN KEY (id_usuario) REFERENCES Usuario(id_usuario)
);

-- Tabela de Curtidas
CREATE TABLE Curtida (
    id_curtida SERIAL PRIMARY KEY,
    id_usuario INT NOT NULL,
    id_postagem INT NOT NULL,
    data_curtida TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    UNIQUE (id_usuario, id_postagem),
    FOREIGN KEY (id_usuario) REFERENCES Usuario(id_usuario),
    FOREIGN KEY (id_postagem) REFERENCES Postagem(id_postagem)
);

-- Tabela de Eventos
CREATE TABLE Evento (
    id_evento SERIAL PRIMARY KEY,
    id_criador INT NOT NULL,
    nome VARCHAR(100) NOT NULL,
    local VARCHAR(255),
    data_evento TIMESTAMP NOT NULL,
    FOREIGN KEY (id_criador) REFERENCES Usuario(id_usuario)
);

-- Tabela de Junção Usuario_Participa_Evento (Relação M:N)
CREATE TABLE Usuario_Participa_Evento (
    id_usuario INT NOT NULL,
    id_evento INT NOT NULL,
    data_inscricao TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (id_usuario, id_evento),
    FOREIGN KEY (id_usuario) REFERENCES Usuario(id_usuario),
    FOREIGN KEY (id_evento) REFERENCES Evento(id_evento)
);

-- Tabela de Bloqueios
CREATE TABLE Bloqueio (
    id_usuario_bloqueador INT NOT NULL,
    id_usuario_bloqueado INT NOT NULL,
    motivo TEXT,
    data_bloqueio TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    status VARCHAR(50) DEFAULT 'ativo',
    PRIMARY KEY (id_usuario_bloqueador, id_usuario_bloqueado),
    FOREIGN KEY (id_usuario_bloqueador) REFERENCES Usuario(id_usuario),
    FOREIGN KEY (id_usuario_bloqueado) REFERENCES Usuario(id_usuario)
);

-- Tabela de Grupos
CREATE TABLE Grupo (
    id_grupo SERIAL PRIMARY KEY,
    id_administrador INT NOT NULL,
    nome VARCHAR(100) NOT NULL,
    descricao TEXT,
    data_criacao TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    privacidade VARCHAR(50),
    FOREIGN KEY (id_administrador) REFERENCES Usuario(id_usuario)
);

-- Tabela de Junção MembroGrupo (Relação M:N entre Usuário e Grupo)
CREATE TABLE MembroGrupo (
    id_usuario INT NOT NULL,
    id_grupo INT NOT NULL,
    data_entrada TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    funcao VARCHAR(50) DEFAULT 'membro',
    PRIMARY KEY (id_usuario, id_grupo),
    FOREIGN KEY (id_usuario) REFERENCES Usuario(id_usuario),
    FOREIGN KEY (id_grupo) REFERENCES Grupo(id_grupo)
);