#include "crow.h"

int main() {

    crow::SimpleApp app;

    CROW_ROUTE(app, "/health")
    ([](){

        return "API funcionando";

    });

    app.port(18080)
       .multithreaded()
       .run();

    return 0;
}
