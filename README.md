# TaskManager API

API REST para gerenciamento de tarefas com autenticação JWT, escrita em C++20 usando Crow, libpqxx e jwt-cpp.

---

## Requisitos

| Ferramenta | Versão mínima | Observação |
|---|---|---|
| CMake | 3.16 | Incluso no WinLibs |
| GCC / G++ | 15.x | Via [WinLibs](https://winlibs.com/) (`C:\winlibs\mingw64`) |
| Ninja | 1.x | Incluso no WinLibs |
| vcpkg | qualquer | Gerenciador de dependências |
| PostgreSQL | 14+ | Banco de dados da aplicação |

---

## Instalação das dependências

### 1. Instalar o vcpkg (apenas na primeira vez)

```powershell
git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat -disableMetrics
```

### 2. Configurar o CMake

No diretório raiz do projeto, execute:

```powershell
cmake -B build `
  -G "Ninja" `
  -DCMAKE_MAKE_PROGRAM="C:/winlibs/mingw64/bin/ninja.exe" `
  -DCMAKE_C_COMPILER="C:/winlibs/mingw64/bin/gcc.exe" `
  -DCMAKE_CXX_COMPILER="C:/winlibs/mingw64/bin/g++.exe" `
  -DCMAKE_TOOLCHAIN_FILE="C:/vcpkg/scripts/buildsystems/vcpkg.cmake" `
  -DVCPKG_TARGET_TRIPLET="x64-mingw-static" `
  -DVCPKG_HOST_TRIPLET="x64-mingw-static" `
  -DCMAKE_BUILD_TYPE=Debug
```

> **Nota:** Na primeira execução o vcpkg compilará automaticamente todas as dependências declaradas em `vcpkg.json` (Crow, jwt-cpp, libpqxx, gtest, picojson). Isso pode levar alguns minutos.

---

## Compilar e executar a aplicação

```powershell
# Compilar a API
cmake --build build --target TaskManager --parallel

# Executar (requer PostgreSQL rodando — veja a seção Banco de Dados)
.\build\TaskManager.exe
```

A API sobe na porta **18080**.

---

## Banco de dados

### Criar banco e tabelas

Conecte-se como superusuário e execute:

```sql
CREATE DATABASE taskdb;

\c taskdb

CREATE TABLE users (
    id    SERIAL       PRIMARY KEY,
    nome  VARCHAR(255) NOT NULL,
    email VARCHAR(255) NOT NULL UNIQUE,
    senha VARCHAR(255) NOT NULL
);

CREATE TABLE tasks (
    id               SERIAL      PRIMARY KEY,
    title            VARCHAR(255) NOT NULL,
    description      TEXT         NOT NULL,
    status           VARCHAR(50)  NOT NULL DEFAULT 'pendente'
                         CHECK (status IN ('pendente', 'em_andamento', 'concluida')),
    assigned_user_id INTEGER      REFERENCES users(id) ON DELETE SET NULL
);
```

### Credenciais de conexão

Por padrão a aplicação conecta em `localhost:5432 / taskdb / postgres`. Para sobrescrever, defina variáveis de ambiente antes de iniciar:

```powershell
$env:PG_HOST = "localhost"
$env:PG_PORT = "5432"
$env:PG_DB   = "taskdb"
$env:PG_USER = "postgres"
$env:PG_PASS = "sua_senha"
.\build\TaskManager.exe
```

---

## Documentação interativa (Swagger)

Com o servidor rodando, abra no navegador:

```
http://localhost:18080/docs
```

Você verá a interface **Swagger UI** com todos os endpoints documentados. Para testar rotas protegidas diretamente pelo Swagger:

1. Clique em **POST /login** → **Try it out** → preencha email e senha → **Execute**
2. Copie o valor do campo `token` da resposta
3. Clique no botão **Authorize** (cadeado no topo da página)
4. Cole o token no campo **Value** e clique **Authorize**
5. Agora qualquer rota com o ícone de cadeado enviará o header `Authorization: Bearer <token>` automaticamente

O JSON bruto da spec OpenAPI 3.0 está em `http://localhost:18080/docs/openapi.json`.

---

## Testando a API manualmente

### 1. Verificar saúde da API e conexão com o banco

```powershell
Invoke-RestMethod http://localhost:18080/health
```

Resposta esperada:
```json
{ "status": "ok", "database": "connected" }
```

### 2. Cadastrar um usuário

```powershell
Invoke-RestMethod http://localhost:18080/register `
  -Method POST `
  -ContentType "application/json" `
  -Body '{"nome":"Carlos Silva","email":"carlos@email.com","senha":"secreta123"}'
```

### 3. Fazer login e guardar o token

```powershell
$resp  = Invoke-RestMethod http://localhost:18080/login `
           -Method POST `
           -ContentType "application/json" `
           -Body '{"email":"carlos@email.com","senha":"secreta123"}'
$token = $resp.token
```

### 4. Criar uma tarefa (requer token)

```powershell
Invoke-RestMethod http://localhost:18080/tarefas `
  -Method POST `
  -ContentType "application/json" `
  -Headers @{ Authorization = "Bearer $token" } `
  -Body '{"titulo":"Implementar tela de login","descricao":"Criar o formulario com validacao","status":"pendente","idUsuarioResponsavel":1}'
```

### 5. Listar todas as tarefas

```powershell
Invoke-RestMethod http://localhost:18080/tarefas
```

### 6. Buscar tarefa por ID

```powershell
Invoke-RestMethod http://localhost:18080/tarefas/1
```

### 7. Atualizar uma tarefa (requer token)

```powershell
Invoke-RestMethod http://localhost:18080/tarefas/1 `
  -Method PUT `
  -ContentType "application/json" `
  -Headers @{ Authorization = "Bearer $token" } `
  -Body '{"titulo":"Implementar tela de login","descricao":"Concluido","status":"concluida","idUsuarioResponsavel":1}'
```

### 8. Remover uma tarefa (requer token)

```powershell
Invoke-RestMethod http://localhost:18080/tarefas/1 `
  -Method DELETE `
  -Headers @{ Authorization = "Bearer $token" }
```

---

## Rotas disponíveis

| Método | Rota | Autenticação | Descrição |
|---|---|---|---|
| `GET` | `/health` | Não | Verifica saúde da API e conexão com o banco |
| `POST` | `/register` | Não | Cadastra novo usuário |
| `POST` | `/login` | Não | Autentica e retorna token JWT |
| `GET` | `/tarefas` | Não | Lista todas as tarefas |
| `GET` | `/tarefas/:id` | Não | Busca tarefa por ID |
| `POST` | `/tarefas` | JWT | Cria nova tarefa |
| `PUT` | `/tarefas/:id` | JWT | Atualiza uma tarefa |
| `DELETE` | `/tarefas/:id` | JWT | Remove uma tarefa |
| `GET` | `/docs` | Não | Swagger UI |
| `GET` | `/docs/openapi.json` | Não | Spec OpenAPI 3.0 |

Rotas protegidas exigem o header:
```
Authorization: Bearer <token>
```

---

## Suíte de testes

O projeto usa **Google Test + Google Mock** como framework de testes. Os testes cobrem as camadas de domínio, aplicação e segurança sem depender de banco de dados.

### 1. Compilar o binário de testes

```powershell
cmake --build build --target TaskManagerTests --parallel
```

O binário gerado fica em `build/TaskManagerTests.exe`.

### 2. Executar todos os testes

```powershell
ctest --test-dir build --output-on-failure
```

Saída esperada:

```
Test project C:/Users/.../build
 1/27 Test  #1: MiddlewareJwtTest.SemHeaderRetornaFalso ............... Passed
 2/27 Test  #2: MiddlewareJwtTest.HeaderSemBearerRetornaFalso ......... Passed
...
27/27 Test #27: UsuarioTest.AtributosPodemSerAtribuidos ............... Passed

100% tests passed, 0 tests failed out of 27
Total Test time (real) =   0.38 sec
```

### 3. Ver detalhes de cada teste (modo verbose)

```powershell
ctest --test-dir build --output-on-failure -V
```

Exibe o nome completo, status e tempo de cada caso de teste individualmente.

### 4. Filtrar por grupo ou nome

```powershell
# Apenas testes de JWT
ctest --test-dir build -R JwtServiceTest --output-on-failure

# Apenas testes do middleware
ctest --test-dir build -R MiddlewareJwtTest --output-on-failure

# Apenas testes de serviços
ctest --test-dir build -R "ServicoTarefaTest|ServicoUsuarioTest" --output-on-failure
```

O padrão passado em `-R` é uma expressão regular aplicada ao nome do teste.

### 5. Recompilar e testar em um único comando

Útil após alterar o código fonte:

```powershell
cmake --build build --target TaskManagerTests --parallel && ctest --test-dir build --output-on-failure
```

### O que cada arquivo de teste cobre

| Arquivo | Classe testada | Casos |
|---|---|---|
| `tests/test_entidades.cpp` | `Tarefa`, `Usuario` | Construtores e atribuição de atributos |
| `tests/test_servico_tarefa.cpp` | `ServicoTarefa` | salvar, buscar, atualizar e remover via mock |
| `tests/test_servico_usuario.cpp` | `ServicoUsuario` | cadastrar, login correto, senha errada, email inexistente |
| `tests/test_jwt_service.cpp` | `JwtService` | gerar token, validar token válido/inválido/corrompido, extrair ID |
| `tests/test_middleware_jwt.cpp` | `MiddlewareJwt` | sem header, sem Bearer, token inválido, token válido |

### Cobertura de código (opcional — requer GCC + lcov)

```powershell
# Reconfigurar com cobertura habilitada
cmake -B build ... -DENABLE_COVERAGE=ON

cmake --build build --target TaskManagerTests --parallel
ctest --test-dir build

# Gerar relatório HTML (excluindo infraestrutura de BD)
lcov --capture --directory build --output-file coverage.info
lcov --remove coverage.info "*/infraestrutura/*" "*/main.cpp" "*/tests/*" `
     -o coverage_filtered.info
genhtml coverage_filtered.info --output-directory coverage_html
```

---

## Estrutura do projeto

```
API/
│
├── main.cpp                          # Ponto de entrada — monta e inicia o servidor
├── CMakeLists.txt                    # Build system
├── vcpkg.json                        # Dependências gerenciadas pelo vcpkg
│
├── src/
│   │
│   ├── apresentacao/
│   │   └── controladores/
│   │       ├── ControladorTarefa.hpp/.cpp    # Rotas HTTP de tarefas (GET/POST/PUT/DELETE)
│   │       ├── ControladorUsuario.hpp/.cpp   # Rotas HTTP de autenticação (register/login)
│   │       └── ControladorDocs.hpp/.cpp      # Swagger UI e spec OpenAPI (/docs)
│   │
│   ├── nucleo/
│   │   ├── dominio/
│   │   │   ├── entidades/
│   │   │   │   ├── Tarefa.hpp        # Entidade de domínio: Tarefa
│   │   │   │   └── Usuario.hpp       # Entidade de domínio: Usuario
│   │   │   └── repositorios/
│   │   │       ├── IRepositorioTarefa.hpp    # Interface (contrato) do repositório de tarefas
│   │   │       └── IRepositorioUsuario.hpp   # Interface (contrato) do repositório de usuários
│   │   │
│   │   └── aplicacao/
│   │       └── servicos/
│   │           ├── ServicoTarefa.hpp/.cpp    # Casos de uso de tarefas
│   │           └── ServicoUsuario.hpp/.cpp   # Casos de uso de usuários (cadastro e login)
│   │
│   ├── infraestrutura/
│   │   ├── banco/
│   │   │   └── BancoDados.hpp/.cpp           # Conexão PostgreSQL via libpqxx
│   │   └── repositorios/
│   │       ├── RepositorioTarefaPostgres.hpp/.cpp   # CRUD de tarefas no PostgreSQL
│   │       └── RepositorioUsuarioPostgres.hpp/.cpp  # Persistência de usuários no PostgreSQL
│   │
│   └── seguranca/
│       ├── JwtService.hpp/.cpp       # Geração e validação de tokens JWT
│       └── MiddlewareJwt.hpp/.cpp    # Middleware de autenticação via header Authorization
│
└── tests/
    ├── test_entidades.cpp            # Testes das entidades Tarefa e Usuario
    ├── test_servico_tarefa.cpp       # Testes de ServicoTarefa com mock do repositório
    ├── test_servico_usuario.cpp      # Testes de ServicoUsuario com mock do repositório
    ├── test_jwt_service.cpp          # Testes de geração, validação e decodificação de JWT
    └── test_middleware_jwt.cpp       # Testes do middleware de autenticação
```

### Arquitetura em camadas (Clean Architecture)

```
┌─────────────────────────────────────────┐
│         Apresentação (HTTP / Crow)       │  ControladorTarefa, ControladorUsuario, ControladorDocs
├─────────────────────────────────────────┤
│         Segurança                        │  JwtService, MiddlewareJwt
├─────────────────────────────────────────┤
│         Aplicação (Casos de uso)         │  ServicoTarefa, ServicoUsuario
├─────────────────────────────────────────┤
│         Domínio (Entidades + Interfaces) │  Tarefa, Usuario, IRepositorio*
├─────────────────────────────────────────┤
│         Infraestrutura (PostgreSQL)      │  RepositorioPostgres, BancoDados
└─────────────────────────────────────────┘
```

As setas de dependência apontam sempre para dentro: o domínio não conhece nenhuma outra camada. A injeção de dependências é feita manualmente no `main.cpp`.

---

## Dependências (vcpkg.json)

| Pacote | Versão instalada | Uso |
|---|---|---|
| [Crow](https://crowcpp.org/) | 1.3.2 | Framework HTTP |
| [jwt-cpp](https://thalhammer.it/projects/jwt_cpp) | 0.7.2 | Geração e validação de JWT |
| [libpqxx](https://pqxx.org/) | 8.0.1 | Cliente C++ para PostgreSQL |
| [picojson](https://github.com/kazuho/picojson) | — | Backend JSON para jwt-cpp |
| [GTest / GMock](https://github.com/google/googletest) | 1.14 | Framework de testes unitários |
