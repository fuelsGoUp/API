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

    set_payload_claim(
      "id",
      jwt::claim(
        std::to_string(
          idUsuario
        )
      )
    )
