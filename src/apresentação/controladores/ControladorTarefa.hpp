#pragma once

#include "crow.h"

#include "../../nucleo/aplicacao/servicos/ServicoTarefa.hpp"

class ControladorTarefa {
public:

    static void registrarRotas(
        crow::SimpleApp& app
    );

private:

    static ServicoTarefa servico;
};
