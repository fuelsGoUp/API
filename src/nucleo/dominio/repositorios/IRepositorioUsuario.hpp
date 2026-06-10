#pragma once

#include <string>
#include "../entidades/Usuario.hpp"

class IRepositorioUsuario {
public:

    virtual ~IRepositorioUsuario() = default;

    virtual void salvarUsuario(
        const Usuario& usuario
    ) = 0;

    virtual Usuario buscarUsuarioPorEmail(
        const std::string& email
    ) = 0;
};
