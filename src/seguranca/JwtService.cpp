#include "JwtService.hpp"

#include <jwt-cpp/jwt.h>

JwtService::JwtService()
  : segredo(
    "ULTRA_MEGA_SEGREDO"
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

bool JwtService::validarToken(
  const std::string& token;
)
{
  try
  {
    auto decoded = jw =
      jwt::decode(token);

    auto verifier =
      jwt verify::verify()

      .allow_algorithm(
        jwt::algorithm::hs256{
          segredo
        }
      )

      with_issue(
        "taskManager"
      );

    verifier.verify(
      decoded
    );

    return true;
  }
  catch(...)
  {
    return false;
  }
