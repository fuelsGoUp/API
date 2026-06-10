#include "ControladorTarefa.hpp"

#include "../../nucleo/dominio/excecoes/Excecoes.hpp"
#include "../../infraestrutura/logger/Logger.hpp"

ControladorTarefa::ControladorTarefa(
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
    // GET /tarefas

    CROW_ROUTE(app, "/tarefas")
    ([this]()
    {
        Logger::info("GET /tarefas");

        try
        {
            auto tarefas = servico.buscarTodas();

            crow::json::wvalue resposta;
            int indice = 0;

            for (const auto& tarefa : tarefas)
            {
                resposta[indice]["id"]       = tarefa.id;
                resposta[indice]["titulo"]   = tarefa.titulo;
                resposta[indice]["descricao"] = tarefa.descricao;
                resposta[indice]["status"]   = tarefa.status;
                indice++;
            }

            return crow::response(200, resposta);
        }
        catch (const std::exception& e)
        {
            Logger::error(std::string("GET /tarefas - erro interno: ") + e.what());
            return crow::response(500, "Erro interno do servidor");
        }
    });

    // GET /tarefas/<id>

    CROW_ROUTE(app, "/tarefas/<int>")
    ([this](int id)
    {
        Logger::info("GET /tarefas/" + std::to_string(id));

        try
        {
            auto tarefa = servico.buscarPorId(id);

            crow::json::wvalue resposta;
            resposta["id"]       = tarefa.id;
            resposta["titulo"]   = tarefa.titulo;
            resposta["descricao"] = tarefa.descricao;
            resposta["status"]   = tarefa.status;

            return crow::response(200, resposta);
        }
        catch (const ExcecaoNaoEncontrado& e)
        {
            Logger::warn(std::string("GET /tarefas/") + std::to_string(id) + " - nao encontrada");
            return crow::response(404, e.what());
        }
        catch (const std::exception& e)
        {
            Logger::error(std::string("GET /tarefas/") + std::to_string(id) + " - erro interno: " + e.what());
            return crow::response(500, "Erro interno do servidor");
        }
    });

    // PUT /tarefas/<id>

    CROW_ROUTE(app, "/tarefas/<int>")
    .methods(crow::HTTPMethod::PUT)
    ([this](const crow::request& req, int id)
    {
        Logger::info("PUT /tarefas/" + std::to_string(id));

        if (!middlewareJwt.autorizado(req))
        {
            Logger::warn("PUT /tarefas/" + std::to_string(id) + " - nao autorizado");
            return crow::response(401, "Nao autorizado");
        }

        try
        {
            auto body = crow::json::load(req.body);

            if (!body)
            {
                return crow::response(400, "JSON invalido");
            }

            if (!body.has("titulo") || !body.has("descricao") || !body.has("status"))
            {
                return crow::response(400, "Campos obrigatorios: titulo, descricao, status");
            }

            Tarefa tarefa;
            tarefa.id                   = id;
            tarefa.titulo               = body["titulo"].s();
            tarefa.descricao            = body["descricao"].s();
            tarefa.status               = body["status"].s();
            tarefa.idUsuarioResponsavel = body["idUsuarioResponsavel"].i();

            servico.atualizar(tarefa);

            return crow::response(200);
        }
        catch (const ExcecaoNaoEncontrado& e)
        {
            Logger::warn("PUT /tarefas/" + std::to_string(id) + " - nao encontrada");
            return crow::response(404, e.what());
        }
        catch (const ExcecaoDadosInvalidos& e)
        {
            return crow::response(400, e.what());
        }
        catch (const std::exception& e)
        {
            Logger::error("PUT /tarefas/" + std::to_string(id) + " - erro interno: " + std::string(e.what()));
            return crow::response(500, "Erro interno do servidor");
        }
    });

    // DELETE /tarefas/<id>

    CROW_ROUTE(app, "/tarefas/<int>")
    .methods(crow::HTTPMethod::DELETE)
    ([this](const crow::request& req, int id)
    {
        Logger::info("DELETE /tarefas/" + std::to_string(id));

        if (!middlewareJwt.autorizado(req))
        {
            Logger::warn("DELETE /tarefas/" + std::to_string(id) + " - nao autorizado");
            return crow::response(401, "Nao autorizado");
        }

        try
        {
            servico.remover(id);

            return crow::response(204);
        }
        catch (const ExcecaoNaoEncontrado& e)
        {
            Logger::warn("DELETE /tarefas/" + std::to_string(id) + " - nao encontrada");
            return crow::response(404, e.what());
        }
        catch (const std::exception& e)
        {
            Logger::error("DELETE /tarefas/" + std::to_string(id) + " - erro interno: " + std::string(e.what()));
            return crow::response(500, "Erro interno do servidor");
        }
    });
}
