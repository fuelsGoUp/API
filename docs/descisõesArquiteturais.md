# Arquitetura

Neste projeto, foi utilizada a Arquitetura Hexagonal em camadas, sendo elas:

## Apresentação

Responsável pelas requisições HTTP.

Exemplo:

* ControladorTarefa

## Aplicação

Responsável pelas regras de negócio.

Exemplo:

* ServicoTarefa

## Domínio

Responsável pelas entidades e contratos.

Exemplos:

* Tarefa
* IRepositorioTarefa

## Infraestrutura

Responsável pelo acesso ao banco de dados.

Exemplos:

* BancoDados
* RepositorioTarefaPostgres

# Fluxo da Aplicação

Cliente
↓
Controller
↓
Service
↓
Repository
↓
PostgreSQL
