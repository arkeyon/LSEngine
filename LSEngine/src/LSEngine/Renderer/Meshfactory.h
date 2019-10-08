#pragma once

#include "LSEngine/Core.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace LSE
{
	struct vertex_t
	{
		glm::vec3 a_Position;
		glm::vec4 a_Colour;
		glm::vec2 a_UV;
		float a_Tex;
	};

	class LSE_API MeshFactory
	{
	public:
		static void generateSphere(const float& radius, const int& detail, glm::vec3* vertices, int32_t* indices, int* numofvertices, int* numofindices, const int& indiceoffs, glm::bvec4 color);
		static void generateRectCorner(vertex_t* vertices, uint32_t* indices, float width, float height, float depth);
		static void generateRectCenter(vertex_t* vertices, uint32_t* indices, float width, float height, float depth);
	};
}