#include "crow.h"

#include "infraestrutura/banco/BancoDados.hpp"
#include "infraestrutura/repositorios/RepositorioTarefaPostgres.hpp"

#include "nucleo/aplicacao/servicos/ServicoTarefa.hpp"

#include "apresentacao/controladores/ControladorTarefa.hpp"

int main()
{
    BancoDados banco;

    RepositorioTarefaPostgres repositorio(
        banco
    );

    ServicoTarefa servico(
        repositorio
    );

    ControladorTarefa controlador(
        servico
    );

    crow::SimpleApp app;

    CROW_ROUTE(app, "/health")
    ([]()
    {
        return "API funcionando";
    });

    controlador.registrarRotas(app);

    app.port(18080)
       .multithreaded()
       .run();

    return 0;
}
