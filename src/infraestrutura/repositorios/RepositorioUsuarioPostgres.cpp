#include "RepositorioUsuarioPostgres.hpp"

#include "../../nucleo/dominio/excecoes/Excecoes.hpp"
#include "../../infraestrutura/logger/Logger.hpp"

RepositorioUsuarioPostgres::RepositorioUsuarioPostgres(
    BancoDados& banco
)
    : banco(banco)
{
}

void RepositorioUsuarioPostgres::salvarUsuario(
    const Usuario& usuario
)
{
    try
    {
        pqxx::work transacao(banco.obterConexao());

        transacao.exec_params(
            "INSERT INTO users (nome, email, senha) "
            "VALUES ($1, $2, $3)",
            usuario.nome,
            usuario.email,
            usuario.senha
        );

        transacao.commit();

        Logger::info("Usuario cadastrado: " + usuario.email);
    }
    catch (const pqxx::unique_violation& e)
    {
        Logger::warn("Tentativa de cadastro com email ja existente: " + usuario.email);
        throw ExcecaoConflito("Email ja cadastrado: " + usuario.email);
    }
    catch (const pqxx::sql_error& e)
    {
        Logger::error("Erro SQL ao salvar usuario: " + std::string(e.what()));
        throw;
    }
}

Usuario
RepositorioUsuarioPostgres::buscarUsuarioPorEmail(
    const std::string& email
)
{
    try
    {
        pqxx::work transacao(banco.obterConexao());

        auto resultado = transacao.exec_params(
            "SELECT * FROM users WHERE email = $1",
            email
        );

        if (resultado.empty())
        {
            Logger::warn("Usuario nao encontrado: " + email);
            throw ExcecaoNaoEncontrado("Usuario nao encontrado: " + email);
        }

        Usuario usuario;
        usuario.id    = resultado[0]["id"].as<int>();
        usuario.nome  = resultado[0]["nome"].c_str();
        usuario.email = resultado[0]["email"].c_str();
        usuario.senha = resultado[0]["senha"].c_str();

        return usuario;
    }
    catch (const ExcecaoNaoEncontrado&)
    {
        throw;
    }
    catch (const pqxx::sql_error& e)
    {
        Logger::error("Erro SQL ao buscar usuario por email: " + std::string(e.what()));
        throw;
    }
}
