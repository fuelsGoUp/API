cmake_minimum_required(VERSION 3.16)

project(TaskManager)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Crow     CONFIG REQUIRED)
find_package(libpqxx  CONFIG REQUIRED)
find_package(jwt-cpp  CONFIG REQUIRED)
find_package(spdlog   CONFIG REQUIRED)

add_executable(TaskManager
    main.cpp
    src/apresentacao/controladores/ControladorTarefa.cpp
    src/apresentacao/controladores/ControladorUsuario.cpp
    src/apresentacao/controladores/ControladorDocs.cpp
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
    jwt-cpp::jwt-cpp
    spdlog::spdlog
    libpqxx::pqxx
    # libpgcommon (transitive via libpq) depends on libpgport symbols, but GNU
    # ld processes archives left-to-right: pgport appears before pgcommon so its
    # symbols are not yet "demanded". --whole-archive forces all pgport symbols
    # in before pgcommon is scanned. --allow-multiple-definition silences the
    # duplicate getopt/getopt_long between libpgport and libmingwex.
    -Wl,--whole-archive
    "${CMAKE_BINARY_DIR}/vcpkg_installed/x64-mingw-static/lib/libpgport.a"
    -Wl,--no-whole-archive,--allow-multiple-definition
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
    spdlog::spdlog
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
