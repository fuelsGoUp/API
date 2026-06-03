#pragma once

#include "../../dominio/repositorios/IRepositorioUsuario.hpp"

class ServicoUsuario
{
private:

    IRepositorioUsuario& repositorio;

public:

    explicit ServicoUsuario(
        IRepositorioUsuario& repositorio
    );

    void cadastrar(
        const Usuario& usuario
    );

    Usuario login(
        const std::string& email,
        const std::string& senha
    );
};
