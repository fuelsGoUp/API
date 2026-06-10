// src/nucleo/dominio/repositorios/IRepositorioTarefa.hpp

#pragma once

#include "../entidades/Tarefa.hpp"

#include <vector>

class IRepositorioTarefa {
public:

    virtual void salvarTarefa(const Tarefa& tarefa) = 0;

    virtual std::vector<Tarefa> buscarTodasTarefas() = 0;

    virtual Tarefa buscarTarefaPorId(int id) = 0;

    virtual void removerTarefa(int id) = 0;

    virtual void atualizarTarefa(const Tarefa& tarefa) = 0;

    virtual ~IRepositorioTarefa() = default;
};
