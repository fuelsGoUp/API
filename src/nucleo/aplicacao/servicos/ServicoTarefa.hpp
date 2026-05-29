#pragma once

#include "../../dominio/entidades/Tarefa.hpp"

#include <vector>

class ServicoTarefa {
public:

    std::vector<Tarefa> buscarTodas();
};
