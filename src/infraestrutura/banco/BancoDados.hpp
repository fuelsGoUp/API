#pragma once

#include <pqxx/pqxx>

class BancoDados {
private:

    pqxx::connection conexao;

public:

    BancoDados()
        : conexao(
            "host=localhost "
            "port=5432 "
            "dbname=taskdb "
            "user=postgres "
            "password=postgres"
        )
    {
    }

    pqxx::connection& obterConexao() {
        return conexao;
    }
};
