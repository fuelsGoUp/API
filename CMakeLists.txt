cmake_minimum_required(VERSION 3.16)

project(TaskManager)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Crow CONFIG REQUIRED)
find_package(libpqxx CONFIG REQUIRED)

add_executable(TaskManager

    src/main.cpp

    src/apresentacao/controladores/ControladorTarefa.cpp

    src/nucleo/aplicacao/servicos/ServicoTarefa.cpp

    src/infraestrutura/banco/BancoDados.cpp

    src/infraestrutura/repositorios/RepositorioTarefaPostgres.cpp
)

target_link_libraries(TaskManager

    PRIVATE

    Crow::Crow
    libpqxx::pqxx
)
