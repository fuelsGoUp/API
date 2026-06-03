#include "ServicoUsuario.hpp"

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
    repositorio.salvar(usuario);
}

Usuario ServicoUsuario::login(
    const std::string& email,
    const std::string& senha
)
{
    Usuario usuario =
        repositorio.buscarPorEmail(email);

    if(usuario.senha != senha)
    {
        throw std::runtime_error(
            "Senha invalida"
        );
    }

    return usuario;
}
