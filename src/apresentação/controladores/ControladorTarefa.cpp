#include "ControladorTarefa.hpp"

ControladorTarefa::ControladorTarefa(
    ServicoTarefa& servico
)
    : servico(servico)
{
}

void ControladorTarefa::registrarRotas(
    crow::SimpleApp& app
)
{
    CROW_ROUTE(app, "/tarefas")
    ([this]()
    {
        auto tarefas =
            servico.buscarTodas();

        crow::json::wvalue resposta;

        int indice = 0;

        for(const auto& tarefa : tarefas)
        {
            resposta[indice]["id"] =
                tarefa.id;

            resposta[indice]["titulo"] =
                tarefa.titulo;

            resposta[indice]["descricao"] =
                tarefa.descricao;

            resposta[indice]["status"] =
                tarefa.status;

            indice++;
        }

        return resposta;
    });
}
