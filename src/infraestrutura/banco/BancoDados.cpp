#include "BancoDados.hpp"
#include "../logger/Logger.hpp"

#include <cstdlib>

BancoDados::BancoDados()
{
    auto env = [](const char* name, const char* fallback) -> std::string {
        const char* val = std::getenv(name);
        return val ? val : fallback;
    };

    connString =
        "host="     + env("PG_HOST", "localhost") +
        " port="    + env("PG_PORT", "5432")      +
        " dbname="  + env("PG_DB",   "taskdb")    +
        " user="    + env("PG_USER", "postgres")  +
        " password=" + env("PG_PASS", "superuser");

    Logger::info("BancoDados configurado (conexao lazy)");
}

pqxx::connection&
BancoDados::obterConexao()
{
    if (!conexao.has_value() || !conexao->is_open())
    {
        Logger::info("Conectando ao banco de dados...");
        conexao.emplace(connString);
        Logger::info("Conexao com o banco de dados estabelecida");
    }

    return conexao.value();
}

bool
BancoDados::testarConexao()
{
    try
    {
        pqxx::work txn(obterConexao());
        txn.exec("SELECT 1");
        return true;
    }
    catch (const std::exception& e)
    {
        Logger::error(std::string("Falha ao testar conexao: ") + e.what());
        return false;
    }
}
