#pragma once

#include "JwtService.hpp"

#include <crow.h>

class MiddlewareJwt
{
private:

    JwtService& jwtService;

public:

    explicit MiddlewareJwt(
        JwtService& jwtService
    );

    bool autorizado(
        const crow::request& req
    );
};
