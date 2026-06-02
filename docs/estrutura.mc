# Estrutura de Diretórios da API

src/

├── apresentacao/
│   └── controladores/
│
├── nucleo/
│   ├── dominio/
│   │   ├── entidades/
│   │   └── repositorios/
│   │
│   └── aplicacao/
│       └── servicos/
│
└── infraestrutura/
├── banco/
└── repositorios/

### Controladores

Recebem requisições HTTP e retornam respostas.

### Serviços

Implementam regras de negócio.

### Entidades

Representam objetos do domínio.

### Repositórios

Realizam operações de persistência.

### Banco de Dados

Gerencia a conexão com PostgreSQL.
