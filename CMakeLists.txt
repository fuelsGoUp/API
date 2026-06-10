cmake_minimum_required(VERSION 3.16)

project(TaskManager)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Crow     CONFIG REQUIRED)
find_package(libpqxx  CONFIG REQUIRED)
find_package(jwt-cpp  CONFIG REQUIRED)

add_executable(TaskManager
    main.cpp
    src/apresentacao/controladores/ControladorTarefa.cpp
    src/apresentacao/controladores/ControladorUsuario.cpp
    src/nucleo/aplicacao/servicos/ServicoTarefa.cpp
    src/nucleo/aplicacao/servicos/ServicoUsuario.cpp
    src/infraestrutura/banco/BancoDados.cpp
    src/infraestrutura/repositorios/RepositorioTarefaPostgres.cpp
    src/infraestrutura/repositorios/RepositorioUsuarioPostgres.cpp
    src/seguranca/JwtService.cpp
    src/seguranca/MiddlewareJwt.cpp
)

target_include_directories(TaskManager PRIVATE src)

target_link_libraries(TaskManager
    PRIVATE
    Crow::Crow
    libpqxx::pqxx
    jwt-cpp::jwt-cpp
)

find_package(GTest CONFIG REQUIRED)

enable_testing()

add_executable(TaskManagerTests
    tests/test_entidades.cpp
    tests/test_servico_tarefa.cpp
    tests/test_servico_usuario.cpp
    tests/test_jwt_service.cpp
    tests/test_middleware_jwt.cpp

    src/nucleo/aplicacao/servicos/ServicoTarefa.cpp
    src/nucleo/aplicacao/servicos/ServicoUsuario.cpp
    src/seguranca/JwtService.cpp
    src/seguranca/MiddlewareJwt.cpp
)

target_include_directories(TaskManagerTests PRIVATE src)

target_link_libraries(TaskManagerTests
    PRIVATE
    GTest::gtest_main
    GTest::gmock
    Crow::Crow
    jwt-cpp::jwt-cpp
)


option(ENABLE_COVERAGE "Gerar dados de cobertura de código" OFF)

if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(TaskManagerTests PRIVATE --coverage -O0 -g)
        target_link_options(TaskManagerTests PRIVATE --coverage)
    endif()
endif()

include(GoogleTest)
gtest_discover_tests(TaskManagerTests)
