#include "BancoDados.hpp"

BancoDados::BancoDados()
    : conexao(
        "host=localhost "
        "port=5432 "
        "dbname=taskdb "
        "user=postgres "
        "password=SUA_SENHA"
    )
{
}

pqxx::connection&
BancoDados::obterConexao() {

    return conexao;
}
