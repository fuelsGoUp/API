#include "RepositorioTarefaPostgres.hpp"

#include <stdexcept>

RepositorioTarefaPostgres::
RepositorioTarefaPostgres(
    BancoDados& banco
)
    : banco(banco)
{
}

void RepositorioTarefaPostgres::salvarTarefa(
    const Tarefa& tarefa
) {

    pqxx::work transacao(
        banco.obterConexao()
    );

    transacao.exec_params(
        "INSERT INTO tasks "
        "(title, description, status, assigned_user_id) "
        "VALUES ($1, $2, $3, $4)",

        tarefa.titulo,
        tarefa.descricao,
        tarefa.status,
        tarefa.idUsuarioResponsavel
    );

    transacao.commit();
}

std::vector<Tarefa>
RepositorioTarefaPostgres::buscarTodasTarefas()
{
    pqxx::work transacao(
        banco.obterConexao()
    );

    pqxx::result resultado =
        transacao.exec(
            "SELECT * FROM tasks"
        );

    std::vector<Tarefa> tarefas;

    for (const auto& linha : resultado) {

        Tarefa tarefa;

        tarefa.id =
            linha["id"].as<int>();

        tarefa.titulo =
            linha["title"].as<std::string>();

        tarefa.descricao =
            linha["description"].as<std::string>();

        tarefa.status =
            linha["status"].as<std::string>();

        tarefa.idUsuarioResponsavel =
            linha["assigned_user_id"].as<int>();

        tarefas.push_back(tarefa);
    }

    return tarefas;
}

Tarefa
RepositorioTarefaPostgres::buscarTarefaPorId(
    int id
)
{
    pqxx::work transacao(
        banco.obterConexao()
    );

    pqxx::result resultado =
        transacao.exec_params(
            "SELECT * "
            "FROM tasks "
            "WHERE id = $1",
            id
        );

    if (resultado.empty()) {

        throw std::runtime_error(
            "Tarefa nao encontrada"
        );
    }

    auto linha = resultado[0];

    Tarefa tarefa;

    tarefa.id =
        linha["id"].as<int>();

    tarefa.titulo =
        linha["title"].as<std::string>();

    tarefa.descricao =
        linha["description"].as<std::string>();

    tarefa.status =
        linha["status"].as<std::string>();

    tarefa.idUsuarioResponsavel =
        linha["assigned_user_id"].as<int>();

    return tarefa;
}

void
RepositorioTarefaPostgres::removerTarefa(
    int id
)
{
    pqxx::work transacao(
        banco.obterConexao()
    );

    transacao.exec_params(
        "DELETE FROM tasks "
        "WHERE id = $1",
        id
    );

    transacao.commit();
}#include "RepositorioTarefaPostgres.hpp"

#include <stdexcept>

RepositorioTarefaPostgres::
RepositorioTarefaPostgres(
    BancoDados& banco
)
    : banco(banco)
{
}

void RepositorioTarefaPostgres::salvarTarefa(
    const Tarefa& tarefa
) {

    pqxx::work transacao(
        banco.obterConexao()
    );

    transacao.exec_params(
        "INSERT INTO tasks "
        "(title, description, status, assigned_user_id) "
        "VALUES ($1, $2, $3, $4)",

        tarefa.titulo,
        tarefa.descricao,
        tarefa.status,
        tarefa.idUsuarioResponsavel
    );

    transacao.commit();
}

std::vector<Tarefa>
RepositorioTarefaPostgres::buscarTodasTarefas()
{
    pqxx::work transacao(
        banco.obterConexao()
    );

    pqxx::result resultado =
        transacao.exec(
            "SELECT * FROM tasks"
        );

    std::vector<Tarefa> tarefas;

    for (const auto& linha : resultado) {

        Tarefa tarefa;

        tarefa.id =
            linha["id"].as<int>();

        tarefa.titulo =
            linha["title"].as<std::string>();

        tarefa.descricao =
            linha["description"].as<std::string>();

        tarefa.status =
            linha["status"].as<std::string>();

        tarefa.idUsuarioResponsavel =
            linha["assigned_user_id"].as<int>();

        tarefas.push_back(tarefa);
    }

    return tarefas;
}

Tarefa
RepositorioTarefaPostgres::buscarPorId(
    int id
)
{
    pqxx::work transacao(
        banco.obterConexao()
    );

    pqxx::result resultado =
        transacao.exec_params(
            "SELECT * "
            "FROM tasks "
            "WHERE id = $1",
            id
        );

    if(resultado.empty())
    {
        throw std::runtime_error(
            "Tarefa nao encontrada"
        );
    }

    auto linha = resultado[0];

    Tarefa tarefa;

    tarefa.id =
        linha["id"].as<int>();

    tarefa.titulo =
        linha["title"].as<std::string>();

    tarefa.descricao =
        linha["description"].as<std::string>();

    tarefa.status =
        linha["status"].as<std::string>();

    tarefa.idUsuarioResponsavel =
        linha["assigned_user_id"].as<int>();

    return tarefa;
}

void
RepositorioTarefaPostgres::remover(
    int id
)
{
    pqxx::work transacao(
        banco.obterConexao()
    );

    transacao.exec_params(
        "DELETE FROM tasks "
        "WHERE id = $1",
        id
    );

    transacao.commit();
}

void RepositorioTarefaPostgres::atualizar(
    const Tarefa& tarefa
)
{
    pqxx::work transacao(
        banco.obterConexao()
    );

    transacao.exec_params(
        "UPDATE tasks "
        "SET title=$1, "
        "description=$2, "
        "status=$3, "
        "assigned_user_id=$4 "
        "WHERE id=$5",

        tarefa.titulo,
        tarefa.descricao,
        tarefa.status,
        tarefa.idUsuarioResponsavel,
        tarefa.id
    );

    transacao.commit();
}
