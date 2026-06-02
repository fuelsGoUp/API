cmake_minimum_required(VERSION 3.16)

project(TaskManager)

set(CMAKE_CXX_STANDARD 20)

find_package(libpqxx CONFIG REQUIRED)

find_package(Crow CONFIG REQUIRED)

target_link_libraries(TaskManager
    Crow::Crow
    libpqxx::pqxx
)

add_executable(TaskManager

    src/main.cpp

    src/apresentacao/controladores/ControladorTarefa.cpp

    src/nucleo/aplicacao/servicos/ServicoTarefa.cpp
)

target_link_libraries(TaskManager
    Crow::Crow
)
