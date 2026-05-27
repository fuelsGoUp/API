#pragma once

#include <string>

class Tarefa {
public:

    int id;
    std::string titulo;
    std::string descricao;
    std::string status;
    int idUsuarioResponsavel;

    Tarefa() = default;

    Tarefa(
        const std::string& titulo,
        const std::string& descricao,
        int idUsuarioResponsavel
    ) {
        this->titulo = titulo;
        this->descricao = descricao;
        this->idUsuarioResponsavel = idUsuarioResponsavel;
        this->status = "pendente";
    }
};
