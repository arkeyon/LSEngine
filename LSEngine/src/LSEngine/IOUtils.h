#pragma once

#include "Core.h"
#include <iostream>

namespace LSE {

	std::string ReadFile(const std::string& filepath);

	uint32_t CreateTexture(const char* filename);

}