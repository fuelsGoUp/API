#include "ControladorTarefa.hpp"

void ControladorTarefa::registrarRotas(
    crow::SimpleApp& app
) {

    CROW_ROUTE(app, "/tarefas")
    ([](){

        crow::json::wvalue resposta;

        resposta[0]["id"] = 1;
        resposta[0]["titulo"] = "Estudar Crow";

        return resposta;
    });
}
