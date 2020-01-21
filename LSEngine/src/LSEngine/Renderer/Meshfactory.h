#pragma once

#include "LSEngine/Core/Core.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace LSE
{
	struct vertex_t
	{
		glm::vec3 a_Position;
		glm::vec4 a_Colour;
		glm::vec3 a_Normal;
		glm::vec2 a_UV;
		float a_Tex;
	};

	class LSE_API MeshFactory
	{
	public:
		static void generateSphere(vertex_t* vertices, uint32_t* indices, const float& radius, const int& detail, vertex_t temp);
		static void generateSphere(vertex_t* vertices, uint32_t* indices, const float& radius, const int& w, const int& h, vertex_t temp);

		static void generateRectCorner(vertex_t* vertices, uint32_t* indices, float width, float height, float depth);
		static void generateRectCenter(vertex_t* vertices, uint32_t* indices, float width, float height, float depth);
		static void generateRectCenterSharedVertices(vertex_t* vertices, uint32_t* indices, float width, float height, float depth);
	};
}