#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "nucleo/dominio/repositorios/IRepositorioTarefa.hpp"
#include "nucleo/aplicacao/servicos/ServicoTarefa.hpp"

using ::testing::Return;
using ::testing::_;

class MockRepositorioTarefa : public IRepositorioTarefa
{
public:
    MOCK_METHOD(void,              salvarTarefa,      (const Tarefa&), (override));
    MOCK_METHOD(std::vector<Tarefa>, buscarTodasTarefas, (),           (override));
    MOCK_METHOD(Tarefa,            buscarTarefaPorId, (int),           (override));
    MOCK_METHOD(void,              removerTarefa,     (int),           (override));
    MOCK_METHOD(void,              atualizarTarefa,   (const Tarefa&), (override));
};

// ── buscarTodas ──────────────────────────────────────────────────────────────

TEST(ServicoTarefaTest, BuscarTodasDelegaAoRepositorio)
{
    MockRepositorioTarefa mock;

    Tarefa t1("Tarefa A", "Desc A", 1);
    Tarefa t2("Tarefa B", "Desc B", 2);

    EXPECT_CALL(mock, buscarTodasTarefas())
        .WillOnce(Return(std::vector<Tarefa>{t1, t2}));

    ServicoTarefa servico(mock);
    auto resultado = servico.buscarTodas();

    ASSERT_EQ(resultado.size(), 2u);
    EXPECT_EQ(resultado[0].titulo, "Tarefa A");
    EXPECT_EQ(resultado[1].titulo, "Tarefa B");
}

TEST(ServicoTarefaTest, BuscarTodasRetornaListaVazia)
{
    MockRepositorioTarefa mock;

    EXPECT_CALL(mock, buscarTodasTarefas())
        .WillOnce(Return(std::vector<Tarefa>{}));

    ServicoTarefa servico(mock);
    auto resultado = servico.buscarTodas();

    EXPECT_TRUE(resultado.empty());
}

// ── buscarPorId ──────────────────────────────────────────────────────────────

TEST(ServicoTarefaTest, BuscarPorIdRetornaTarefaCorreta)
{
    MockRepositorioTarefa mock;

    Tarefa esperada("Minha tarefa", "Desc", 5);
    esperada.id = 3;

    EXPECT_CALL(mock, buscarTarefaPorId(3))
        .WillOnce(Return(esperada));

    ServicoTarefa servico(mock);
    Tarefa resultado = servico.buscarPorId(3);

    EXPECT_EQ(resultado.id, 3);
    EXPECT_EQ(resultado.titulo, "Minha tarefa");
}

// ── salvar ───────────────────────────────────────────────────────────────────

TEST(ServicoTarefaTest, SalvarDelegaAoRepositorio)
{
    MockRepositorioTarefa mock;
    Tarefa tarefa("Nova", "Descricao", 1);

    EXPECT_CALL(mock, salvarTarefa(_)).Times(1);

    ServicoTarefa servico(mock);
    servico.salvar(tarefa);
}

// ── atualizar ────────────────────────────────────────────────────────────────

TEST(ServicoTarefaTest, AtualizarDelegaAoRepositorio)
{
    MockRepositorioTarefa mock;

    Tarefa tarefa("Atualizada", "Nova desc", 2);
    tarefa.id = 10;

    EXPECT_CALL(mock, atualizarTarefa(_)).Times(1);

    ServicoTarefa servico(mock);
    servico.atualizar(tarefa);
}

// ── remover ──────────────────────────────────────────────────────────────────

TEST(ServicoTarefaTest, RemoverDelegaAoRepositorio)
{
    MockRepositorioTarefa mock;

    EXPECT_CALL(mock, removerTarefa(7)).Times(1);

    ServicoTarefa servico(mock);
    servico.remover(7);
}
