#pragma once

#include "crow.h"

#include "../../seguranca/MiddlewareJwt.hpp"

#include "../../nucleo/aplicacao/servicos/ServicoTarefa.hpp"

class ControladorTarefa {
private:

    ServicoTarefa& servico;

public:

    explicit ControladorTarefa(
        ServicoTarefa& servico
    );

    void registrarRotas(
        crow::SimpleApp& app
    );
};#pragma once

#include "crow.h"

#include "../../nucleo/aplicacao/servicos/ServicoTarefa.hpp"

class ControladorTarefa {
private:

    ServicoTarefa& servico;
    MiddlewareJwt& middlewareJwt;

public:

    explicit ControladorTarefa(
        ServicoTarefa& servico
    );

    void registrarRotas(
        crow::SimpleApp& app
    );
};
