#include "ServicoTarefa.hpp"

std::vector<Tarefa>
ServicoTarefa::buscarTodas() {

    std::vector<Tarefa> tarefas;

    Tarefa tarefa;

    tarefa.id = 1;
    tarefa.titulo = "Estudar Crow";
    tarefa.descricao = "Aprender rotas";
    tarefa.status = "pendente";

    tarefas.push_back(tarefa);

    return tarefas;
}
