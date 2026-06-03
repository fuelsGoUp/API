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
    return repositorio.buscarTodas();
}#include "ServicoTarefa.hpp"

ServicoTarefa::ServicoTarefa(
    IRepositorioTarefa& repositorio
)
    : repositorio(repositorio)
{
}

std::vector<Tarefa>
ServicoTarefa::buscarTodas()
{
    return repositorio.buscarTodas();
}

Tarefa
ServicoTarefa::buscarPorId(
    int id
)
{
    return repositorio.buscarPorId(
        id
    );
}
