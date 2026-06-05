#pragma once

#include <string>

class JwtService
{
private:

    std::string segredo;

public:

    JwtService();

    std::string gerarToken(
        int idUsuario,
        const std::string& email
    );

    bool validarToken(
        const std::string& token
    );

    int obterIdUsuario(
        const std::string& token
    );
};
