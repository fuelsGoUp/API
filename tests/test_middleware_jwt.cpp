#include <gtest/gtest.h>
#include <crow.h>

#include "seguranca/JwtService.hpp"
#include "seguranca/MiddlewareJwt.hpp"

// Helpers para montar crow::request com headers
static crow::request makeRequest(const std::string& headerAuth = "")
{
    crow::request req;
    if (!headerAuth.empty())
        req.add_header("Authorization", headerAuth);
    return req;
}

// ── Header ausente ────────────────────────────────────────────────────────────

TEST(MiddlewareJwtTest, SemHeaderRetornaFalso)
{
    JwtService jwt;
    MiddlewareJwt middleware(jwt);

    auto req = makeRequest();
    EXPECT_FALSE(middleware.autorizado(req));
}

// ── Header sem prefixo "Bearer " ─────────────────────────────────────────────

TEST(MiddlewareJwtTest, HeaderSemBearerRetornaFalso)
{
    JwtService jwt;
    MiddlewareJwt middleware(jwt);

    auto req = makeRequest("token_qualquer");
    EXPECT_FALSE(middleware.autorizado(req));
}

TEST(MiddlewareJwtTest, HeaderComPrefixoErradoRetornaFalso)
{
    JwtService jwt;
    MiddlewareJwt middleware(jwt);

    auto req = makeRequest("Basic dXNlcjpwYXNz");
    EXPECT_FALSE(middleware.autorizado(req));
}

// ── Token inválido ────────────────────────────────────────────────────────────

TEST(MiddlewareJwtTest, TokenInvalidoRetornaFalso)
{
    JwtService jwt;
    MiddlewareJwt middleware(jwt);

    auto req = makeRequest("Bearer isto.nao.e.valido");
    EXPECT_FALSE(middleware.autorizado(req));
}

// ── Token válido ──────────────────────────────────────────────────────────────

TEST(MiddlewareJwtTest, TokenValidoRetornaVerdadeiro)
{
    JwtService jwt;
    MiddlewareJwt middleware(jwt);

    std::string token = jwt.gerarToken(1, "user@email.com");
    auto req = makeRequest("Bearer " + token);

    EXPECT_TRUE(middleware.autorizado(req));
}

TEST(MiddlewareJwtTest, TokenDeUsuarioDiferenteAindaEValido)
{
    JwtService jwt;
    MiddlewareJwt middleware(jwt);

    std::string token = jwt.gerarToken(99, "outro@email.com");
    auto req = makeRequest("Bearer " + token);

    EXPECT_TRUE(middleware.autorizado(req));
}
