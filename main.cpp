#include <crow.h>

#include "infraestrutura/banco/BancoDados.hpp"
#include "infraestrutura/logger/Logger.hpp"

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
    Logger::init();

    Logger::info("Iniciando TaskManager API...");

    try
    {
        crow::SimpleApp app;

        BancoDados banco;

        RepositorioTarefaPostgres repositorioTarefa(banco);
        RepositorioUsuarioPostgres repositorioUsuario(banco);

        ServicoTarefa  servicoTarefa(repositorioTarefa);
        ServicoUsuario servicoUsuario(repositorioUsuario);

        JwtService     jwtService;
        MiddlewareJwt  middlewareJwt(jwtService);

        ControladorTarefa  controladorTarefa(servicoTarefa, middlewareJwt);
        ControladorUsuario controladorUsuario(servicoUsuario, jwtService);

        controladorTarefa.registrarRotas(app);
        controladorUsuario.registrarRotas(app);

        Logger::info("Servidor escutando na porta 18080");

        app.port(18080)
           .multithreaded()
           .run();
    }
    catch (const std::exception& e)
    {
        Logger::error(std::string("Falha fatal ao iniciar o servidor: ") + e.what());
        return 1;
    }

    return 0;
}
