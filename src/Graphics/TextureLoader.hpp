#pragma once
#include "../Tools/Log.hpp"
#include "glad/glad.h"
#include "stb_image.h"
#include <cstdint>
#include <string>

uint32_t loadTexture(const std::string &directoryPath, uint32_t filtering);
