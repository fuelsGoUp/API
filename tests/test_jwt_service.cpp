#include <gtest/gtest.h>
#include "seguranca/JwtService.hpp"

// ── gerarToken ───────────────────────────────────────────────────────────────

TEST(JwtServiceTest, GerarTokenRetornaStringNaoVazia)
{
    JwtService jwt;
    std::string token = jwt.gerarToken(1, "user@email.com");

    EXPECT_FALSE(token.empty());
}

TEST(JwtServiceTest, GerarTokenParaDiferentesUsuariosRetornaTokensDiferentes)
{
    JwtService jwt;
    std::string t1 = jwt.gerarToken(1, "a@a.com");
    std::string t2 = jwt.gerarToken(2, "b@b.com");

    EXPECT_NE(t1, t2);
}

// ── validarToken ─────────────────────────────────────────────────────────────

TEST(JwtServiceTest, ValidarTokenGeradoPeloProprioServico)
{
    JwtService jwt;
    std::string token = jwt.gerarToken(10, "user@email.com");

    EXPECT_TRUE(jwt.validarToken(token));
}

TEST(JwtServiceTest, ValidarTokenInvalidoRetornaFalso)
{
    JwtService jwt;
    EXPECT_FALSE(jwt.validarToken("isto.nao.e.um.jwt"));
}

TEST(JwtServiceTest, ValidarTokenVazioRetornaFalso)
{
    JwtService jwt;
    EXPECT_FALSE(jwt.validarToken(""));
}

TEST(JwtServiceTest, ValidarTokenAlteradoRetornaFalso)
{
    JwtService jwt;
    std::string token = jwt.gerarToken(5, "user@email.com");

    // Corrompe um caractere no meio do token
    token[token.size() / 2] ^= 0x01;

    EXPECT_FALSE(jwt.validarToken(token));
}

// ── obterIdUsuario ───────────────────────────────────────────────────────────

TEST(JwtServiceTest, ObterIdUsuarioRetornaIdCorreto)
{
    JwtService jwt;
    std::string token = jwt.gerarToken(42, "user@email.com");

    EXPECT_EQ(jwt.obterIdUsuario(token), 42);
}

TEST(JwtServiceTest, ObterIdUsuarioParaDiferentesIds)
{
    JwtService jwt;

    EXPECT_EQ(jwt.obterIdUsuario(jwt.gerarToken(1,   "a@a.com")), 1);
    EXPECT_EQ(jwt.obterIdUsuario(jwt.gerarToken(999, "b@b.com")), 999);
}
