#pragma once

#include "crow.h"

#include "../../seguranca/MiddlewareJwt.hpp"
#include "../../nucleo/aplicacao/servicos/ServicoTarefa.hpp"

class ControladorTarefa
{
private:

    ServicoTarefa& servico;

    MiddlewareJwt& middlewareJwt;

public:

    ControladorTarefa(
        ServicoTarefa& servico,
        MiddlewareJwt& middlewareJwt
    );

    void registrarRotas(
        crow::SimpleApp& app
    );
};
