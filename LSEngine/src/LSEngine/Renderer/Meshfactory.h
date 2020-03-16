#pragma once

#include "LSEngine/Core/Core.h"
#include "LSEngine/Renderer/Mesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace LSE {

	class LSE_API MeshFactory
	{
	public:
		static Ref<Mesh> generateSphere(const float& radius, const int& detail, glm::vec4 colour = { 1.f, 1.f, 1.f, 1.f });
		static Ref<Mesh> generateSphere(const float& radius, const int& w, const int& h, glm::vec4 colour = {1.f, 1.f, 1.f, 1.f});

		static Ref<Mesh> generateRectCorner(float width, float height, float depth);
		static Ref<Mesh> generateRectCenter(float width, float height, float depth);

		static Ref<Mesh> generateCubeCorner(float size);
		static Ref<Mesh> generateCubeCenter(float size);

		static Ref<Mesh> MeshFactory::generatePlaneCorner(glm::vec3 x, glm::vec3 y);

		static Ref<Mesh> generateRectCenterSharedVertices(float width, float height, float depth);

		//typedef glm::vec3(*curvefunc_t)(const float& t);
		//typedef glm::vec4(*curvecolourfunc_t)(const float& t);
		//static Ref<Mesh> generateCurve(curvefunc_t curvefunc, float tstart = 0.f, float tend = 1.f, const int tsteps = 10, curvecolourfunc_t curvecolourfunc = [](const float& t) { return glm::vec4(1.f, 1.f, 1.f, 1.f); });
		
		typedef glm::vec3(*surfacefunc_t)(const float& u, const float& v);
		typedef glm::vec4(*surfacecolourfunc_t)(const float& u, const float& v);
		static Ref<Mesh> generateSurface(surfacefunc_t curvefunc, float ustart = 0.f, float uend = 1.f, const int usteps = 10, float vstart = 0.f, float vend = 1.f, const int vsteps = 10, surfacecolourfunc_t curvecolourfunc = [](const float& u, const float& v) { return glm::vec4(1.f, 1.f, 1.f, 1.f); });
	};
}