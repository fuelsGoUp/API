#include "ControladorUsuario.hpp"

ControladorUsuario::
ControladorUsuario(
    ServicoUsuario& servico
)
    : servico(servico)
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
        auto json =
            crow::json::load(
                req.body
            );

        Usuario usuario;

        usuario.nome =
            json["nome"].s();

        usuario.email =
            json["email"].s();

        usuario.senha =
            json["senha"].s();

        servico.cadastrar(
            usuario
        );

        return crow::response(
            201
        );
    });

    CROW_ROUTE(app, "/login")
    .methods(crow::HTTPMethod::POST)
    ([this](const crow::request& req)
    {
        auto json =
            crow::json::load(
                req.body
            );

        Usuario usuario =
            servico.login(
                json["email"].s(),
                json["senha"].s()
            );

        crow::json::wvalue resposta;

        resposta["id"] =
            usuario.id;

        resposta["nome"] =
            usuario.nome;

        std::string token =
          jwtService.gerarToken(
          usuario.id,
          usuario.email
    );

        resposta["token"] =
          token;

        return resposta;
    });
}
