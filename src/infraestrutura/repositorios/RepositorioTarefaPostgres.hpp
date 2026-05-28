#pragma once

#include "../../nucleo/dominio/repositorios/IRepositorioTarefa.hpp"
#include "../banco/BancoDados.hpp"

class RepositorioTarefaPostgres : public IRepositorioTarefa {
private:

    BancoDados& banco;

public:

    RepositorioTarefaPostgres(BancoDados& banco);

    void salvar(const Tarefa& tarefa) override;

    std::vector<Tarefa> buscarTodas() override;

    Tarefa buscarPorId(int id) override;

    void remover(int id) override;
};
