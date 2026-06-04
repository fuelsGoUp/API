#pragma once

JwtService&
jwtService;

#include "../../nucleo/aplicacao/servicos/ServicoUsuario.hpp"

#include <crow.h>

class ControladorUsuario
{
private:

    ServicoUsuario& servico;

public:

    explicit ControladorUsuario(
    ServicoUsuario& servico,
    JwtService& jwtService
);

    void registrarRotas(
        crow::SimpleApp& app
    );
};
