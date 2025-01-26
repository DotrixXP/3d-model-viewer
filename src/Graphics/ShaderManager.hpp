#pragma once
#include "Shader.hpp"
#include <cstdint>

enum class ShaderType : uint8_t {
    SIMPLE,
    LIGHT,
    REFLECT
};

struct ShaderBatch {
    Shader reflect;
    Shader lightShader;
    Shader simpleShader;

    ShaderBatch();
};
