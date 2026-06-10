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

    void salvarUsuario(
        const Usuario& usuario
    ) override;

    Usuario buscarUsuarioPorEmail(
        const std::string& email
    ) override;
};
