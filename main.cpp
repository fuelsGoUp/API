#include "crow.h"

#include "apresentacao/controladores/ControladorTarefa.hpp"

int main() {

    crow::SimpleApp app;

    CROW_ROUTE(app, "/health")
    ([](){
        return "API funcionando";
    });

    ControladorTarefa::registrarRotas(app);

    app.port(18080)
       .multithreaded()
       .run();

    return 0;
}
