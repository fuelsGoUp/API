#include "JwtService.hpp"

#include <jwt-cpp/jwt.h>

JwtService::JwtService()
    : segredo(
        "SEGREDO_SUPER_SECRETO_2026"
    )
{
}

std::string JwtService::gerarToken(
    int idUsuario,
    const std::string& email
)
{
    auto token =
        jwt::create()

        .set_type("JWT")

        .set_issuer(
            "TaskManager"
        )

        .set_payload_claim(
            "id",
            jwt::claim(
                std::to_string(
                    idUsuario
                )
            )
        )

        .set_payload_claim(
            "email",
            jwt::claim(
                email
            )
        )

        .sign(
            jwt::algorithm::hs256{
                segredo
            }
        );

    return token;
}
