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

Crie o banco e as tabelas antes de rodar a aplicação:

```sql
CREATE DATABASE taskdb;

\c taskdb

CREATE TABLE users (
    id    SERIAL PRIMARY KEY,
    nome  TEXT NOT NULL,
    email TEXT NOT NULL UNIQUE,
    senha TEXT NOT NULL
);

CREATE TABLE tasks (
    id                     SERIAL PRIMARY KEY,
    titulo                 TEXT NOT NULL,
    descricao              TEXT,
    status                 TEXT NOT NULL DEFAULT 'pendente',
    id_usuario_responsavel INTEGER REFERENCES users(id)
);
```

As credenciais de conexão estão em [src/infraestrutura/banco/BancoDados.cpp](src/infraestrutura/banco/BancoDados.cpp).

---

## Rotas disponíveis

| Método | Rota | Autenticação | Descrição |
|---|---|---|---|
| `POST` | `/register` | Não | Cadastra novo usuário |
| `POST` | `/login` | Não | Autentica e retorna token JWT |
| `GET` | `/tarefas` | Não | Lista todas as tarefas |
| `GET` | `/tarefas/:id` | Não | Busca tarefa por ID |
| `PUT` | `/tarefas/:id` | JWT | Atualiza uma tarefa |
| `DELETE` | `/tarefas/:id` | JWT | Remove uma tarefa |

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
│   │       ├── ControladorTarefa.hpp/.cpp    # Rotas HTTP de tarefas (GET/PUT/DELETE)
│   │       └── ControladorUsuario.hpp/.cpp   # Rotas HTTP de autenticação (register/login)
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
│         Apresentação (HTTP / Crow)       │  ControladorTarefa, ControladorUsuario
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
