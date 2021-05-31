#pragma once

#include <glm/glm.hpp>

#include <array>
#include <vector>

namespace LSE::Maths {

	bool solveQuadQuad(const glm::mat4& A, const glm::mat4& B, glm::vec2& t);

	std::vector<float> solveQuadratic(float a, float b, float c);

}