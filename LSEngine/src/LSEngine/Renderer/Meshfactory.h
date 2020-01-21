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

		typedef glm::vec3(*curvefunc_t)(const float& t);
		typedef glm::vec4(*curvecolourfunc_t)(const float& t);
		static void generateCurve(vertex_t* vertices, uint32_t* indices, curvefunc_t curvefunc, float tstart = 0.f, float tend = 1.f, float tsteps = 10.f, curvecolourfunc_t curvecolourfunc = [](const float& t) { return glm::vec4(1.f, 1.f, 1.f, 1.f); });
		
		typedef glm::vec3(*surfacefunc_t)(const float& u, const float& v);
		typedef glm::vec4(*surfacecolourfunc_t)(const float& u, const float& v);
		static void generateSurface(vertex_t* vertices, uint32_t* indices, surfacefunc_t curvefunc, float ustart = 0.f, float uend = 1.f, float usteps = 10.f, float vstart = 0.f, float vend = 1.f, float vsteps = 10.f, surfacecolourfunc_t curvecolourfunc = [](const float& u, const float& v) { return glm::vec4(1.f, 1.f, 1.f, 1.f); });
	};
}