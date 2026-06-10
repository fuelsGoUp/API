#include <gtest/gtest.h>
#include "nucleo/dominio/entidades/Tarefa.hpp"
#include "nucleo/dominio/entidades/Usuario.hpp"

// ── Tarefa ───────────────────────────────────────────────────────────────────

TEST(TarefaTest, ConstrutorComParametrosDefineAtributos)
{
    Tarefa t("Implementar login", "Criar endpoint POST /login", 42);

    EXPECT_EQ(t.titulo, "Implementar login");
    EXPECT_EQ(t.descricao, "Criar endpoint POST /login");
    EXPECT_EQ(t.idUsuarioResponsavel, 42);
    EXPECT_EQ(t.status, "pendente");
}

TEST(TarefaTest, ConstrutorPadraoPermiteAtribuicaoDireta)
{
    Tarefa t;
    t.id = 1;
    t.titulo = "Tarefa avulsa";
    t.status = "concluida";

    EXPECT_EQ(t.id, 1);
    EXPECT_EQ(t.titulo, "Tarefa avulsa");
    EXPECT_EQ(t.status, "concluida");
}

// ── Usuario ──────────────────────────────────────────────────────────────────

TEST(UsuarioTest, AtributosPodemSerAtribuidos)
{
    Usuario u;
    u.id    = 7;
    u.nome  = "Ana";
    u.email = "ana@email.com";
    u.senha = "secreta";

    EXPECT_EQ(u.id, 7);
    EXPECT_EQ(u.nome, "Ana");
    EXPECT_EQ(u.email, "ana@email.com");
    EXPECT_EQ(u.senha, "secreta");
}
