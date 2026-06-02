#pragma once

#include "crow.h"

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

public:

    explicit ControladorTarefa(
        ServicoTarefa& servico
    );

    void registrarRotas(
        crow::SimpleApp& app
    );
};
