#pragma once

#include <pqxx/pqxx>
#include <optional>
#include <string>

class BancoDados {
private:
    std::string connString;
    std::optional<pqxx::connection> conexao;

public:
    BancoDados();

    pqxx::connection& obterConexao();
    bool testarConexao();
};
