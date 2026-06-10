#pragma once

#include "../../nucleo/aplicacao/servicos/ServicoUsuario.hpp"
#include "../../seguranca/JwtService.hpp"

#include <crow.h>

class ControladorUsuario
{
private:

    ServicoUsuario& servico;
    JwtService& jwtService;

public:

    ControladorUsuario(
        ServicoUsuario& servico,
        JwtService& jwtService
    );

    void registrarRotas(
        crow::SimpleApp& app
    );
};
