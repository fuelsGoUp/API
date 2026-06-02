#pragma once

#include <pqxx/pqxx>

class BancoDados {
private:
    pqxx::connection conexao;

public:
    BancoDados();

    pqxx::connection& obterConexao();
};
