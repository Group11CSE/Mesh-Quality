#pragma once

#include <exception>

namespace Mesh_Quality{
    class MqException: std::exception{
    public:
        const char* what() const noexcept {return "Mesh Quality Module Error";};
    };
}