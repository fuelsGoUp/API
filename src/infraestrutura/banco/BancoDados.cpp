#include "BancoDados.hpp"
#include "../logger/Logger.hpp"

BancoDados::BancoDados()
    : conexao(
        "host=localhost "
        "port=5432 "
        "dbname=taskdb "
        "user=postgres "
        "password=SUA_SENHA"
    )
{
    Logger::info("Conexao com o banco de dados estabelecida");
}

pqxx::connection&
BancoDados::obterConexao()
{
    return conexao;
}
