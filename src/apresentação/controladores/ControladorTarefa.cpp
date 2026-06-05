#include "ControladorTarefa.hpp"

ControladorTarefa::
ControladorTarefa(
    ServicoTarefa& servico,
    MiddlewareJwt& middlewareJwt
)
    : servico(servico),
      middlewareJwt(middlewareJwt)
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

    CROW_ROUTE(app, "/tarefas/<int>")
    .methods(crow::HTTPMethod::PUT)
    ([this](const crow::request& req, int id)
{
    if(!middlewareJwt.autorizado(req))
    {
        return crow::response(
            401,
            "Nao autorizado"
        );
    }
        auto body =
            crow::json::load(req.body);

        Tarefa tarefa;

        tarefa.id = id;

        tarefa.titulo =
            body["titulo"].s();

        tarefa.descricao =
            body["descricao"].s();

        tarefa.status =
            body["status"].s();

        tarefa.idUsuarioResponsavel =
            body["idUsuarioResponsavel"].i();

        servico.atualizar(tarefa);

        return crow::response(200);
    });

    CROW_ROUTE(app, "/tarefas/<int>")
    .methods(crow::HTTPMethod::DELETE)
    ([this](const crow::request& req, int id)
{
    if(!middlewareJwt.autorizado(req))
    {
        return crow::response(
            401,
            "Nao autorizado"
        );
    }
        servico.remover(id);

        return crow::response(204);
});
}
