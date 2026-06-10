#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdexcept>

#include "nucleo/dominio/entidades/Usuario.hpp"
#include "nucleo/dominio/repositorios/IRepositorioUsuario.hpp"
#include "nucleo/aplicacao/servicos/ServicoUsuario.hpp"

using ::testing::Return;
using ::testing::_;

class MockRepositorioUsuario : public IRepositorioUsuario
{
public:
    MOCK_METHOD(void,    salvarUsuario,          (const Usuario&),     (override));
    MOCK_METHOD(Usuario, buscarUsuarioPorEmail,  (const std::string&), (override));
};

// ── cadastrar ────────────────────────────────────────────────────────────────

TEST(ServicoUsuarioTest, CadastrarDelegaAoRepositorio)
{
    MockRepositorioUsuario mock;

    Usuario u;
    u.nome  = "Carlos";
    u.email = "carlos@email.com";
    u.senha = "123456";

    EXPECT_CALL(mock, salvarUsuario(_)).Times(1);

    ServicoUsuario servico(mock);
    servico.cadastrar(u);
}

// ── login ────────────────────────────────────────────────────────────────────

TEST(ServicoUsuarioTest, LoginComCredenciaisCorretas)
{
    MockRepositorioUsuario mock;

    Usuario usuarioNoBanco;
    usuarioNoBanco.id    = 1;
    usuarioNoBanco.email = "carlos@email.com";
    usuarioNoBanco.senha = "secreta";

    EXPECT_CALL(mock, buscarUsuarioPorEmail("carlos@email.com"))
        .WillOnce(Return(usuarioNoBanco));

    ServicoUsuario servico(mock);
    Usuario resultado = servico.login("carlos@email.com", "secreta");

    EXPECT_EQ(resultado.id, 1);
    EXPECT_EQ(resultado.email, "carlos@email.com");
}

TEST(ServicoUsuarioTest, LoginComSenhaErradaLancaExcecao)
{
    MockRepositorioUsuario mock;

    Usuario usuarioNoBanco;
    usuarioNoBanco.email = "carlos@email.com";
    usuarioNoBanco.senha = "secreta";

    EXPECT_CALL(mock, buscarUsuarioPorEmail("carlos@email.com"))
        .WillOnce(Return(usuarioNoBanco));

    ServicoUsuario servico(mock);

    EXPECT_THROW(
        servico.login("carlos@email.com", "senha_errada"),
        std::runtime_error
    );
}

TEST(ServicoUsuarioTest, LoginComEmailInexistentePropagarExcecao)
{
    MockRepositorioUsuario mock;

    EXPECT_CALL(mock, buscarUsuarioPorEmail("nao@existe.com"))
        .WillOnce(testing::Throw(
            std::runtime_error("Usuario nao encontrado")
        ));

    ServicoUsuario servico(mock);

    EXPECT_THROW(
        servico.login("nao@existe.com", "qualquer"),
        std::runtime_error
    );
}
