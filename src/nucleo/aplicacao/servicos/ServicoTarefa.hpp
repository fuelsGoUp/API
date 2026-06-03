#pragma once

#include "../../dominio/repositorios/IRepositorioTarefa.hpp"

class ServicoTarefa {
private:

    IRepositorioTarefa& repositorio;

public:

    explicit ServicoTarefa(
        IRepositorioTarefa& repositorio
    );

    std::vector<Tarefa> buscarTodas();

    Tarefa buscarPorId(
        int id
        );Tarefa buscarPorId(
        int id
    );

    void atualizar(
        const Tarefa& tarefa
    );

    void remover(
        int id
    );
};
