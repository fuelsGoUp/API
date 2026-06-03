#pragma once

#include "../entidades/Usuario.hpp"

class IRepositorioUsuario
{
public:

    virtual ~IRepositorioUsuario() = default;

    virtual void salvar(
        const Usuario& usuario
    ) = 0;

    virtual Usuario buscarPorEmail(
        const std::string& email
    ) = 0;
};
