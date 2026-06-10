#include "ControladorDocs.hpp"

// ── OpenAPI 3.0.3 spec ───────────────────────────────────────────────────────

static const std::string OPENAPI_SPEC = R"({
  "openapi": "3.0.3",
  "info": {
    "title": "TaskManager API",
    "version": "1.0.0",
    "description": "API REST de gerenciamento de tarefas com autenticacao JWT.\n\n**Fluxo basico:**\n1. `POST /register` — cria um usuario\n2. `POST /login` — obtem o token JWT\n3. Passe o token no header `Authorization: Bearer <token>` para as rotas protegidas"
  },
  "servers": [
    { "url": "http://localhost:18080", "description": "Servidor local" }
  ],
  "tags": [
    { "name": "Infra",        "description": "Saude e status da API" },
    { "name": "Autenticacao", "description": "Cadastro e login de usuarios" },
    { "name": "Tarefas",      "description": "CRUD de tarefas" }
  ],
  "paths": {
    "/health": {
      "get": {
        "tags": ["Infra"],
        "summary": "Verificar saude da API e conexao com o banco de dados",
        "responses": {
          "200": {
            "description": "API e banco de dados operacionais",
            "content": {
              "application/json": {
                "schema": { "$ref": "#/components/schemas/HealthResponse" },
                "example": { "status": "ok", "database": "connected" }
              }
            }
          },
          "503": {
            "description": "Banco de dados indisponivel",
            "content": {
              "application/json": {
                "schema": { "$ref": "#/components/schemas/HealthResponse" },
                "example": { "status": "error", "database": "disconnected" }
              }
            }
          }
        }
      }
    },
    "/register": {
      "post": {
        "tags": ["Autenticacao"],
        "summary": "Cadastrar novo usuario",
        "requestBody": {
          "required": true,
          "content": {
            "application/json": {
              "schema": { "$ref": "#/components/schemas/UsuarioInput" },
              "example": {
                "nome": "Carlos Silva",
                "email": "carlos@email.com",
                "senha": "secreta123"
              }
            }
          }
        },
        "responses": {
          "201": { "description": "Usuario criado com sucesso" },
          "400": {
            "description": "Dados invalidos ou campos obrigatorios ausentes",
            "content": { "text/plain": { "schema": { "type": "string" } } }
          },
          "409": {
            "description": "Email ja cadastrado",
            "content": { "text/plain": { "schema": { "type": "string" } } }
          },
          "500": { "description": "Erro interno do servidor" }
        }
      }
    },
    "/login": {
      "post": {
        "tags": ["Autenticacao"],
        "summary": "Autenticar usuario e obter token JWT",
        "requestBody": {
          "required": true,
          "content": {
            "application/json": {
              "schema": { "$ref": "#/components/schemas/LoginInput" },
              "example": {
                "email": "carlos@email.com",
                "senha": "secreta123"
              }
            }
          }
        },
        "responses": {
          "200": {
            "description": "Token JWT gerado com sucesso",
            "content": {
              "application/json": {
                "schema": { "$ref": "#/components/schemas/TokenResponse" }
              }
            }
          },
          "400": { "description": "Dados invalidos ou campos obrigatorios ausentes" },
          "401": { "description": "Credenciais invalidas" },
          "500": { "description": "Erro interno do servidor" }
        }
      }
    },
    "/tarefas": {
      "post": {
        "tags": ["Tarefas"],
        "summary": "Criar nova tarefa",
        "description": "Requer autenticacao JWT via header `Authorization: Bearer <token>`.",
        "security": [{ "BearerAuth": [] }],
        "requestBody": {
          "required": true,
          "content": {
            "application/json": {
              "schema": { "$ref": "#/components/schemas/TarefaInput" },
              "example": {
                "titulo": "Implementar login",
                "descricao": "Criar endpoint de autenticacao com JWT",
                "status": "pendente",
                "idUsuarioResponsavel": 1
              }
            }
          }
        },
        "responses": {
          "201": { "description": "Tarefa criada com sucesso" },
          "400": { "description": "Dados invalidos ou campos obrigatorios ausentes" },
          "401": { "description": "Nao autorizado — token ausente ou invalido" },
          "500": { "description": "Erro interno do servidor" }
        }
      },
      "get": {
        "tags": ["Tarefas"],
        "summary": "Listar todas as tarefas",
        "responses": {
          "200": {
            "description": "Lista de tarefas retornada com sucesso",
            "content": {
              "application/json": {
                "schema": {
                  "type": "array",
                  "items": { "$ref": "#/components/schemas/Tarefa" }
                }
              }
            }
          },
          "500": { "description": "Erro interno do servidor" }
        }
      }
    },
    "/tarefas/{id}": {
      "get": {
        "tags": ["Tarefas"],
        "summary": "Buscar tarefa por ID",
        "parameters": [
          {
            "name": "id",
            "in": "path",
            "required": true,
            "description": "ID da tarefa",
            "schema": { "type": "integer", "example": 1 }
          }
        ],
        "responses": {
          "200": {
            "description": "Tarefa encontrada",
            "content": {
              "application/json": {
                "schema": { "$ref": "#/components/schemas/Tarefa" }
              }
            }
          },
          "404": { "description": "Tarefa nao encontrada" },
          "500": { "description": "Erro interno do servidor" }
        }
      },
      "put": {
        "tags": ["Tarefas"],
        "summary": "Atualizar tarefa",
        "description": "Requer autenticacao JWT via header `Authorization: Bearer <token>`.",
        "security": [{ "BearerAuth": [] }],
        "parameters": [
          {
            "name": "id",
            "in": "path",
            "required": true,
            "description": "ID da tarefa a atualizar",
            "schema": { "type": "integer", "example": 1 }
          }
        ],
        "requestBody": {
          "required": true,
          "content": {
            "application/json": {
              "schema": { "$ref": "#/components/schemas/TarefaInput" },
              "example": {
                "titulo": "Implementar login",
                "descricao": "Criar endpoint de autenticacao com JWT",
                "status": "em_andamento",
                "idUsuarioResponsavel": 2
              }
            }
          }
        },
        "responses": {
          "200": { "description": "Tarefa atualizada com sucesso" },
          "400": { "description": "Dados invalidos ou campos obrigatorios ausentes" },
          "401": { "description": "Nao autorizado — token ausente ou invalido" },
          "404": { "description": "Tarefa nao encontrada" },
          "500": { "description": "Erro interno do servidor" }
        }
      },
      "delete": {
        "tags": ["Tarefas"],
        "summary": "Remover tarefa",
        "description": "Requer autenticacao JWT via header `Authorization: Bearer <token>`.",
        "security": [{ "BearerAuth": [] }],
        "parameters": [
          {
            "name": "id",
            "in": "path",
            "required": true,
            "description": "ID da tarefa a remover",
            "schema": { "type": "integer", "example": 1 }
          }
        ],
        "responses": {
          "204": { "description": "Tarefa removida com sucesso" },
          "401": { "description": "Nao autorizado — token ausente ou invalido" },
          "404": { "description": "Tarefa nao encontrada" },
          "500": { "description": "Erro interno do servidor" }
        }
      }
    }
  },
  "components": {
    "securitySchemes": {
      "BearerAuth": {
        "type": "http",
        "scheme": "bearer",
        "bearerFormat": "JWT",
        "description": "Token JWT obtido via `POST /login`. Insira apenas o token, sem o prefixo 'Bearer'."
      }
    },
    "schemas": {
      "HealthResponse": {
        "type": "object",
        "properties": {
          "status":   { "type": "string", "enum": ["ok", "error"],                   "example": "ok" },
          "database": { "type": "string", "enum": ["connected", "disconnected"], "example": "connected" }
        }
      },
      "UsuarioInput": {
        "type": "object",
        "required": ["nome", "email", "senha"],
        "properties": {
          "nome":  { "type": "string",  "example": "Carlos Silva" },
          "email": { "type": "string",  "format": "email", "example": "carlos@email.com" },
          "senha": { "type": "string",  "format": "password", "example": "secreta123" }
        }
      },
      "LoginInput": {
        "type": "object",
        "required": ["email", "senha"],
        "properties": {
          "email": { "type": "string",  "format": "email",    "example": "carlos@email.com" },
          "senha": { "type": "string",  "format": "password", "example": "secreta123" }
        }
      },
      "TokenResponse": {
        "type": "object",
        "properties": {
          "token": {
            "type": "string",
            "example": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6IjEiLCJlbWFpbCI6ImNhcmxvc0BlbWFpbC5jb20ifQ.signature"
          }
        }
      },
      "Tarefa": {
        "type": "object",
        "properties": {
          "id":                   { "type": "integer", "example": 1 },
          "titulo":               { "type": "string",  "example": "Implementar login" },
          "descricao":            { "type": "string",  "example": "Criar endpoint de autenticacao com JWT" },
          "status": {
            "type": "string",
            "enum": ["pendente", "em_andamento", "concluida"],
            "example": "pendente"
          },
          "idUsuarioResponsavel": { "type": "integer", "example": 2 }
        }
      },
      "TarefaInput": {
        "type": "object",
        "required": ["titulo", "descricao", "status", "idUsuarioResponsavel"],
        "properties": {
          "titulo":               { "type": "string",  "example": "Implementar login" },
          "descricao":            { "type": "string",  "example": "Criar endpoint de autenticacao com JWT" },
          "status": {
            "type": "string",
            "enum": ["pendente", "em_andamento", "concluida"],
            "example": "pendente"
          },
          "idUsuarioResponsavel": { "type": "integer", "example": 2 }
        }
      }
    }
  }
})";

// ── Swagger UI HTML ──────────────────────────────────────────────────────────

static const std::string SWAGGER_HTML = R"(<!DOCTYPE html>
<html lang="pt-BR">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <title>TaskManager API - Documentacao</title>
  <link rel="stylesheet" href="https://unpkg.com/swagger-ui-dist@5/swagger-ui.css" />
  <style>
    body { margin: 0; }
    #swagger-ui .topbar { background-color: #1b1b1b; }
  </style>
</head>
<body>
  <div id="swagger-ui"></div>
  <script src="https://unpkg.com/swagger-ui-dist@5/swagger-ui-bundle.js"></script>
  <script src="https://unpkg.com/swagger-ui-dist@5/swagger-ui-standalone-preset.js"></script>
  <script>
    window.onload = function() {
      SwaggerUIBundle({
        url: "/docs/openapi.json",
        dom_id: "#swagger-ui",
        deepLinking: true,
        presets: [
          SwaggerUIBundle.presets.apis,
          SwaggerUIStandalonePreset
        ],
        plugins: [SwaggerUIBundle.plugins.DownloadUrl],
        layout: "StandaloneLayout"
      });
    };
  </script>
</body>
</html>)";

// ── Rotas ────────────────────────────────────────────────────────────────────

void ControladorDocs::registrarRotas(crow::SimpleApp& app)
{
    CROW_ROUTE(app, "/docs")
    ([]()
    {
        crow::response res(200, SWAGGER_HTML);
        res.add_header("Content-Type", "text/html; charset=utf-8");
        return res;
    });

    CROW_ROUTE(app, "/docs/openapi.json")
    ([]()
    {
        crow::response res(200, OPENAPI_SPEC);
        res.add_header("Content-Type", "application/json");
        res.add_header("Access-Control-Allow-Origin", "*");
        return res;
    });
}
