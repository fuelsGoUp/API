#include <crow.h>

#include "infraestrutura/banco/BancoDados.hpp"

#include "infraestrutura/repositorios/RepositorioTarefaPostgres.hpp"
#include "infraestrutura/repositorios/RepositorioUsuarioPostgres.hpp"

#include "nucleo/aplicacao/servicos/ServicoTarefa.hpp"
#include "nucleo/aplicacao/servicos/ServicoUsuario.hpp"

#include "apresentacao/controladores/ControladorTarefa.hpp"
#include "apresentacao/controladores/ControladorUsuario.hpp"

#include "seguranca/JwtService.hpp"
#include "seguranca/MiddlewareJwt.hpp"

int main()
{
    try
    {
        crow::SimpleApp app;

        BancoDados banco;

        RepositorioTarefaPostgres
            repositorioTarefa(banco);

        RepositorioUsuarioPostgres
            repositorioUsuario(banco);

        ServicoTarefa
            servicoTarefa(
                repositorioTarefa
            );

        ServicoUsuario
            servicoUsuario(
                repositorioUsuario
            );

        JwtService jwtService;

        MiddlewareJwt
            middlewareJwt(
                jwtService
            );

        ControladorTarefa
            controladorTarefa(
                servicoTarefa,
                middlewareJwt
            );

        ControladorUsuario
            controladorUsuario(
                servicoUsuario,
                jwtService
            );

        controladorTarefa
            .registrarRotas(app);

        controladorUsuario
            .registrarRotas(app);

        app.port(18080)
           .multithreaded()
           .run();
    }
    catch(const std::exception& e)
    {
        std::cerr
            << e.what()
            << std::endl;

        return 1;
    }

    return 0;
}
