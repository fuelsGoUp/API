cmake_minimum_required(VERSION 3.16)

project(TaskManager)

set(CMAKE_CXX_STANDARD 20)

find_package(Crow CONFIG REQUIRED)

add_executable(TaskManager

    src/main.cpp

    src/apresentacao/controladores/ControladorTarefa.cpp
)

target_link_libraries(TaskManager
    Crow::Crow
)
