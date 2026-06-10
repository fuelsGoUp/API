#pragma once

#include "../../nucleo/dominio/repositorios/IRepositorioTarefa.hpp"
#include "../banco/BancoDados.hpp"

class RepositorioTarefaPostgres
    : public IRepositorioTarefa {

private:

    BancoDados& banco;

public:

    explicit RepositorioTarefaPostgres(
        BancoDados& banco
    );

    void salvarTarefa(
        const Tarefa& tarefa
    ) override;

    std::vector<Tarefa>
    buscarTodasTarefas() override;

    Tarefa buscarTarefaPorId(
        int id
    ) override;

    void removerTarefa(
        int id
    ) override;

    void atualizarTarefa(
        const Tarefa& tarefa
    ) override;
};
