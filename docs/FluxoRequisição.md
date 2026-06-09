# Documentação dos endpoints da API 

## Health Check

### GET /health

Verifica se a API está em funcionamento.

Resposta:

API funcionando

---

## Listar Tarefas

### GET /tarefas

Retorna todas as tarefas cadastradas.

Exemplo de resposta:

[
{
"id": 1,
"titulo": "Estudar Crow",
"descricao": "Criar API REST",
"status": "pendente"
}
]

---

## Buscar Tarefa por ID

### GET /tarefas/{id}

Retorna uma tarefa específica.

Exemplo:

GET /tarefas/1

Resposta:

{
"id": 1,
"titulo": "Estudar Crow",
"descricao": "Criar API REST",
"status": "pendente"
}

---

## Criar Tarefa

### POST /tarefas

Cria uma nova tarefa.

Exemplo de corpo:

{
"titulo": "Estudar Arquitetura",
"descricao": "Implementar CRUD",
"status": "pendente",
"idUsuarioResponsavel": 1
}

Resposta:

HTTP 201 Created
