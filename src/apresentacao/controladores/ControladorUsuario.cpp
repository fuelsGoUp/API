#include "ControladorUsuario.hpp"

#include "../../nucleo/dominio/excecoes/Excecoes.hpp"
#include "../../infraestrutura/logger/Logger.hpp"

ControladorUsuario::ControladorUsuario(
    ServicoUsuario& servico,
    JwtService& jwtService
)
    : servico(servico),
      jwtService(jwtService)
{
}

void ControladorUsuario::registrarRotas(
    crow::SimpleApp& app
)
{
    CROW_ROUTE(app, "/register")
    .methods(crow::HTTPMethod::POST)
    ([this](const crow::request& req)
    {
        Logger::info("POST /register");

        try
        {
            auto json = crow::json::load(req.body);

            if (!json)
            {
                return crow::response(400, "JSON invalido");
            }

            if (!json.has("nome") || !json.has("email") || !json.has("senha"))
            {
                return crow::response(400, "Campos obrigatorios: nome, email, senha");
            }

            Usuario usuario;
            usuario.nome  = json["nome"].s();
            usuario.email = json["email"].s();
            usuario.senha = json["senha"].s();

            servico.cadastrar(usuario);

            return crow::response(201);
        }
        catch (const ExcecaoDadosInvalidos& e)
        {
            return crow::response(400, e.what());
        }
        catch (const ExcecaoConflito& e)
        {
            Logger::warn(std::string("POST /register - conflito: ") + e.what());
            return crow::response(409, e.what());
        }
        catch (const std::exception& e)
        {
            Logger::error(std::string("POST /register - erro interno: ") + e.what());
            return crow::response(500, "Erro interno do servidor");
        }
    });

    CROW_ROUTE(app, "/login")
    .methods(crow::HTTPMethod::POST)
    ([this](const crow::request& req)
    {
        Logger::info("POST /login");

        try
        {
            auto json = crow::json::load(req.body);

            if (!json)
            {
                return crow::response(400, "JSON invalido");
            }

            if (!json.has("email") || !json.has("senha"))
            {
                return crow::response(400, "Campos obrigatorios: email, senha");
            }

            Usuario usuario = servico.login(
                json["email"].s(),
                json["senha"].s()
            );

            std::string token = jwtService.gerarToken(
                usuario.id,
                usuario.email
            );

            crow::json::wvalue resposta;
            resposta["token"] = token;

            Logger::info("Login bem-sucedido: " + usuario.email);

            return crow::response(200, resposta);
        }
        catch (const ExcecaoDadosInvalidos& e)
        {
            return crow::response(400, e.what());
        }
        catch (const ExcecaoNaoAutorizado& e)
        {
            Logger::warn("POST /login - credenciais invalidas");
            return crow::response(401, e.what());
        }
        catch (const ExcecaoNaoEncontrado&)
        {
            Logger::warn("POST /login - credenciais invalidas");
            return crow::response(401, "Credenciais invalidas");
        }
        catch (const std::exception& e)
        {
            Logger::error(std::string("POST /login - erro interno: ") + e.what());
            return crow::response(500, "Erro interno do servidor");
        }
    });
}
