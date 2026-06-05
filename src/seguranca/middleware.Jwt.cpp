#include "MiddlewareJwt.hpp"

MiddlewareJwt::MiddlewareJwt(
    JwtService& jwtService
)
    : jwtService(jwtService)
{
}

bool MiddlewareJwt::autorizado(
    const crow::request& req
)
{
    auto cabecalho =
        req.get_header_value(
            "Authorization"
        );

    if(cabecalho.empty())
    {
        return false;
    }

    const std::string prefixo =
        "Bearer ";

    if(cabecalho.find(prefixo) != 0)
    {
        return false;
    }

    std::string token =
        cabecalho.substr(
            prefixo.size()
        );

    return jwtService
        .validarToken(token);
}
