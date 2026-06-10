#pragma once

#include <stdexcept>
#include <string>

class ExcecaoNaoEncontrado : public std::runtime_error {
public:
    explicit ExcecaoNaoEncontrado(const std::string& msg)
        : std::runtime_error(msg) {}
};

class ExcecaoNaoAutorizado : public std::runtime_error {
public:
    explicit ExcecaoNaoAutorizado(const std::string& msg = "Nao autorizado")
        : std::runtime_error(msg) {}
};

class ExcecaoDadosInvalidos : public std::runtime_error {
public:
    explicit ExcecaoDadosInvalidos(const std::string& msg)
        : std::runtime_error(msg) {}
};

class ExcecaoConflito : public std::runtime_error {
public:
    explicit ExcecaoConflito(const std::string& msg)
        : std::runtime_error(msg) {}
};
