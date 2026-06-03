#include <crow.h>

#include "infraestrutura/banco/BancoDados.hpp"

#include "infraestrutura/repositorios/RepositorioTarefaPostgres.hpp"
#include "infraestrutura/repositorios/RepositorioUsuarioPostgres.hpp"

#include "nucleo/aplicacao/servicos/ServicoTarefa.hpp"
#include "nucleo/aplicacao/servicos/ServicoUsuario.hpp"

#include "apresentacao/controladores/ControladorTarefa.hpp"
#include "apresentacao/controladores/ControladorUsuario.hpp"

int main()
{
    try
    {
        crow::SimpleApp app;

        // Banco de dados

        BancoDados banco;

        // Repositórios

        RepositorioTarefaPostgres
            repositorioTarefa(banco);

        RepositorioUsuarioPostgres
            repositorioUsuario(banco);

        // Serviços

        ServicoTarefa
            servicoTarefa(
                repositorioTarefa
            );

        ServicoUsuario
            servicoUsuario(
                repositorioUsuario
            );

        // Controladores

        ControladorTarefa
            controladorTarefa(
                servicoTarefa
            );

        ControladorUsuario
            controladorUsuario(
                servicoUsuario
            );

        // Rotas

        controladorTarefa
            .registrarRotas(app);

        controladorUsuario
            .registrarRotas(app);

        std::cout
            << "Servidor iniciado em:"
            << std::endl;

        std::cout
            << "http://localhost:18080"
            << std::endl;

        app.port(18080)
           .multithreaded()
           .run();
    }
    catch(const std::exception& e)
    {
        std::cerr
            << "Erro: "
            << e.what()
            << std::endl;

        return 1;
    }

    return 0;
}
