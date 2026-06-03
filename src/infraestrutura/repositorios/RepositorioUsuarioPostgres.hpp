#pragma once

#include "../../nucleo/dominio/repositorios/IRepositorioUsuario.hpp"
#include "../banco/BancoDados.hpp"

class RepositorioUsuarioPostgres
    : public IRepositorioUsuario
{
private:

    BancoDados& banco;

public:

    explicit RepositorioUsuarioPostgres(
        BancoDados& banco
    );

    void salvar(
        const Usuario& usuario
    ) override;

    Usuario buscarPorEmail(
        const std::string& email
    ) override;
};
