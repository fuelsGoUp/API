# Sistema de Gerenciamento de Tarefas

## Objetivo

Desenvolver uma API REST para gerenciamento de tarefas utilizando C++, PostgreSQL e Crow Framework.

## Tecnologias Utilizadas

* Linguagem: C++20
* Framework Web: Crow
* Banco de Dados: PostgreSQL
* Biblioteca de Acesso ao Banco: libpqxx
* Build System: CMake
* IDE: Visual Studio Code

## Arquitetura

O sistema segue uma arquitetura em camadas:

### Apresentação

Responsável pelas requisições HTTP.

Exemplo:

* ControladorTarefa

### Aplicação

Responsável pelas regras de negócio.

Exemplo:

* ServicoTarefa

### Domínio

Responsável pelas entidades e contratos.

Exemplos:

* Tarefa
* IRepositorioTarefa

### Infraestrutura

Responsável pelo acesso ao banco de dados.

Exemplos:

* BancoDados
* RepositorioTarefaPostgres

## Fluxo da Aplicação

Cliente
↓
Controller
↓
Service
↓
Repository
↓
PostgreSQL
