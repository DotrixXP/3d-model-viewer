#pragma once
#include "../Tools/Log.hpp"
#include "glad/glad.h"
#include "stb_image.h"
#include <cstdint>
#include <string>

uint32_t loadTexture(const std::string &directoryPath, uint32_t filtering);
void updateTexture(uint32_t texture, const std::string &directoryPath);
