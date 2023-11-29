#pragma once
#include <string>
#include <cstdint>
#include "glad/glad.h"
#include "../Tools/Log.hpp"
#include "stb_image.h"

uint32_t loadTexture(const std::string& directoryPath, uint32_t filtering);
