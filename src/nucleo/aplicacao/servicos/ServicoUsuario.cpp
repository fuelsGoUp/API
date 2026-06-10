#include "ServicoUsuario.hpp"

#include "../../../nucleo/dominio/excecoes/Excecoes.hpp"

ServicoUsuario::ServicoUsuario(
    IRepositorioUsuario& repositorio
)
    : repositorio(repositorio)
{
}

void ServicoUsuario::cadastrar(
    const Usuario& usuario
)
{
    if (usuario.nome.empty() || usuario.email.empty() || usuario.senha.empty())
    {
        throw ExcecaoDadosInvalidos("Nome, email e senha sao obrigatorios");
    }

    repositorio.salvarUsuario(usuario);
}

Usuario ServicoUsuario::login(
    const std::string& email,
    const std::string& senha
)
{
    if (email.empty() || senha.empty())
    {
        throw ExcecaoDadosInvalidos("Email e senha sao obrigatorios");
    }

    Usuario usuario = repositorio.buscarUsuarioPorEmail(email);

    if (usuario.senha != senha)
    {
        throw ExcecaoNaoAutorizado("Credenciais invalidas");
    }

    return usuario;
}
