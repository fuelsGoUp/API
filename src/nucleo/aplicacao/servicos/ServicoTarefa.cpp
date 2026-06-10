#include "ServicoTarefa.hpp"

ServicoTarefa::ServicoTarefa(
    IRepositorioTarefa& repositorio
)
    : repositorio(repositorio)
{
}

std::vector<Tarefa>
ServicoTarefa::buscarTodas()
{
    return repositorio.buscarTodasTarefas();
}

Tarefa
ServicoTarefa::buscarPorId(
    int id
)
{
    return repositorio.buscarTarefaPorId(id);
}

void ServicoTarefa::salvar(
    const Tarefa& tarefa
)
{
    repositorio.salvarTarefa(tarefa);
}

void ServicoTarefa::atualizar(
    const Tarefa& tarefa
)
{
    repositorio.atualizarTarefa(tarefa);
}

void ServicoTarefa::remover(
    int id
)
{
    repositorio.removerTarefa(id);
}
