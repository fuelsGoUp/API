#include "ControladorTarefa.hpp"

ServicoTarefa ControladorTarefa::servico;

void ControladorTarefa::registrarRotas(
    crow::SimpleApp& app
) {

    CROW_ROUTE(app, "/tarefas")
    ([](){

        auto tarefas =
            servico.buscarTodas();

        crow::json::wvalue resposta;

        int index = 0;

        for (const auto& tarefa : tarefas) {

            resposta[index]["id"] =
                tarefa.id;

            resposta[index]["titulo"] =
                tarefa.titulo;

            resposta[index]["descricao"] =
                tarefa.descricao;

            resposta[index]["status"] =
                tarefa.status;

            index++;
        }

        return resposta;
    });
}
