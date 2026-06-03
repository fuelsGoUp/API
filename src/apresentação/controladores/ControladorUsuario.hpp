#pragma once

#include "../../nucleo/aplicacao/servicos/ServicoUsuario.hpp"

#include <crow.h>

class ControladorUsuario
{
private:

    ServicoUsuario& servico;

public:

    explicit ControladorUsuario(
        ServicoUsuario& servico
    );

    void registrarRotas(
        crow::SimpleApp& app
    );
};
