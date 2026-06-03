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

            indice++;
        }

        return resposta;
    });

    CROW_ROUTE(app, "/tarefas/<int>")
    ([this](int id)
    {
        auto tarefa =
            servico.buscarPorId(id);

        crow::json::wvalue resposta;

        resposta["id"] =
            tarefa.id;

        resposta["titulo"] =
            tarefa.titulo;

        resposta["descricao"] =
            tarefa.descricao;

        resposta["status"] =
            tarefa.status;

        return resposta;
    });
}
