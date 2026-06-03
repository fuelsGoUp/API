#include "RepositorioUsuarioPostgres.hpp"

RepositorioUsuarioPostgres::
RepositorioUsuarioPostgres(
    BancoDados& banco
)
    : banco(banco)
{
}

void RepositorioUsuarioPostgres::salvar(
    const Usuario& usuario
)
{
    pqxx::work transacao(
        banco.obterConexao()
    );

    transacao.exec_params(
        "INSERT INTO users "
        "(nome,email,senha) "
        "VALUES($1,$2,$3)",

        usuario.nome,
        usuario.email,
        usuario.senha
    );

    transacao.commit();
}

Usuario
RepositorioUsuarioPostgres::buscarPorEmail(
    const std::string& email
)
{
    pqxx::work transacao(
        banco.obterConexao()
    );

    auto resultado =
        transacao.exec_params(
            "SELECT * "
            "FROM users "
            "WHERE email=$1",
            email
        );

    if(resultado.empty())
    {
        throw std::runtime_error(
            "Usuario nao encontrado"
        );
    }

    Usuario usuario;

    usuario.id =
        resultado[0]["id"].as<int>();

    usuario.nome =
        resultado[0]["nome"].c_str();

    usuario.email =
        resultado[0]["email"].c_str();

    usuario.senha =
        resultado[0]["senha"].c_str();

    return usuario;
}
