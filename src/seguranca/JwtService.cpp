#include "JwtService.hpp"

#include <jwt-cpp/traits/kazuho-picojson/defaults.h>

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
    using namespace std::chrono;

    auto token =
        jwt::create()

        .set_type("JWT")

        .set_issuer(
            "TaskManager"
        )

        .set_expires_at(
            system_clock::now()
            + hours(24)
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

bool JwtService::validarToken(
    const std::string& token
)
{
    try
    {
        auto decoded =
            jwt::decode(token);

        auto verifier =
            jwt::verify()

            .allow_algorithm(
                jwt::algorithm::hs256{
                    segredo
                }
            )

            .with_issuer(
                "TaskManager"
            );

        verifier.verify(
            decoded);

        return true;
    }
    catch(...)
    {
        return false;
    }
}

int JwtService::obterIdUsuario(
    const std::string& token
)
{
    auto decoded =
        jwt::decode(token);

    return std::stoi(
        decoded
            .get_payload_claim("id")
            .as_string()
    );
}
