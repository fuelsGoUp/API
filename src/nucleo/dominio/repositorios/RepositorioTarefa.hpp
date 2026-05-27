// src/nucleo/dominio/repositorios/IRepositorioTarefa.hpp

#pragma once

#include "../entidades/Tarefa.hpp"

#include <vector>

class IRepositorioTarefa {
public:

    virtual void salvar(const Tarefa& tarefa) = 0;

    virtual std::vector<Tarefa> buscarTodas() = 0;

    virtual Tarefa buscarPorId(int id) = 0;

    virtual void remover(int id) = 0;

    virtual ~IRepositorioTarefa() = default;
};
