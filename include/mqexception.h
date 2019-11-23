#pragma once

#include <exception>

namespace Mesh_Quality{
    class MqException: std::exception{
    public:
        virtual const char* what() = 0;
    };
}