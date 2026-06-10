#include "RepositorioTarefaPostgres.hpp"

#include "../../nucleo/dominio/excecoes/Excecoes.hpp"
#include "../../infraestrutura/logger/Logger.hpp"

RepositorioTarefaPostgres::RepositorioTarefaPostgres(
    BancoDados& banco
)
    : banco(banco)
{
}

void RepositorioTarefaPostgres::salvarTarefa(
    const Tarefa& tarefa
)
{
    try
    {
        pqxx::work transacao(banco.obterConexao());

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

        Logger::info("Tarefa salva: " + tarefa.titulo);
    }
    catch (const pqxx::sql_error& e)
    {
        Logger::error("Erro SQL ao salvar tarefa: " + std::string(e.what()));
        throw;
    }
}

std::vector<Tarefa>
RepositorioTarefaPostgres::buscarTodasTarefas()
{
    try
    {
        pqxx::work transacao(banco.obterConexao());

        pqxx::result resultado = transacao.exec(
            "SELECT * FROM tasks"
        );

        std::vector<Tarefa> tarefas;

        for (const auto& linha : resultado)
        {
            Tarefa tarefa;
            tarefa.id                  = linha["id"].as<int>();
            tarefa.titulo              = linha["title"].as<std::string>();
            tarefa.descricao           = linha["description"].as<std::string>();
            tarefa.status              = linha["status"].as<std::string>();
            tarefa.idUsuarioResponsavel = linha["assigned_user_id"].as<int>();
            tarefas.push_back(tarefa);
        }

        Logger::debug("buscarTodasTarefas: " + std::to_string(tarefas.size()) + " tarefas retornadas");

        return tarefas;
    }
    catch (const pqxx::sql_error& e)
    {
        Logger::error("Erro SQL ao buscar tarefas: " + std::string(e.what()));
        throw;
    }
}

Tarefa
RepositorioTarefaPostgres::buscarTarefaPorId(int id)
{
    try
    {
        pqxx::work transacao(banco.obterConexao());

        pqxx::result resultado = transacao.exec_params(
            "SELECT * FROM tasks WHERE id = $1",
            id
        );

        if (resultado.empty())
        {
            Logger::warn("Tarefa nao encontrada: id=" + std::to_string(id));
            throw ExcecaoNaoEncontrado(
                "Tarefa com id " + std::to_string(id) + " nao encontrada"
            );
        }

        auto linha = resultado[0];

        Tarefa tarefa;
        tarefa.id                  = linha["id"].as<int>();
        tarefa.titulo              = linha["title"].as<std::string>();
        tarefa.descricao           = linha["description"].as<std::string>();
        tarefa.status              = linha["status"].as<std::string>();
        tarefa.idUsuarioResponsavel = linha["assigned_user_id"].as<int>();

        return tarefa;
    }
    catch (const ExcecaoNaoEncontrado&)
    {
        throw;
    }
    catch (const pqxx::sql_error& e)
    {
        Logger::error("Erro SQL ao buscar tarefa id=" + std::to_string(id) + ": " + e.what());
        throw;
    }
}

void
RepositorioTarefaPostgres::atualizarTarefa(const Tarefa& tarefa)
{
    try
    {
        pqxx::work transacao(banco.obterConexao());

        pqxx::result resultado = transacao.exec_params(
            "UPDATE tasks "
            "SET title=$1, description=$2, status=$3, assigned_user_id=$4 "
            "WHERE id=$5",
            tarefa.titulo,
            tarefa.descricao,
            tarefa.status,
            tarefa.idUsuarioResponsavel,
            tarefa.id
        );

        if (resultado.affected_rows() == 0)
        {
            Logger::warn("Tentativa de atualizar tarefa inexistente: id=" + std::to_string(tarefa.id));
            throw ExcecaoNaoEncontrado(
                "Tarefa com id " + std::to_string(tarefa.id) + " nao encontrada"
            );
        }

        transacao.commit();

        Logger::info("Tarefa atualizada: id=" + std::to_string(tarefa.id));
    }
    catch (const ExcecaoNaoEncontrado&)
    {
        throw;
    }
    catch (const pqxx::sql_error& e)
    {
        Logger::error("Erro SQL ao atualizar tarefa id=" + std::to_string(tarefa.id) + ": " + e.what());
        throw;
    }
}

void
RepositorioTarefaPostgres::removerTarefa(int id)
{
    try
    {
        pqxx::work transacao(banco.obterConexao());

        pqxx::result resultado = transacao.exec_params(
            "DELETE FROM tasks WHERE id = $1",
            id
        );

        if (resultado.affected_rows() == 0)
        {
            Logger::warn("Tentativa de remover tarefa inexistente: id=" + std::to_string(id));
            throw ExcecaoNaoEncontrado(
                "Tarefa com id " + std::to_string(id) + " nao encontrada"
            );
        }

        transacao.commit();

        Logger::info("Tarefa removida: id=" + std::to_string(id));
    }
    catch (const ExcecaoNaoEncontrado&)
    {
        throw;
    }
    catch (const pqxx::sql_error& e)
    {
        Logger::error("Erro SQL ao remover tarefa id=" + std::to_string(id) + ": " + e.what());
        throw;
    }
}
