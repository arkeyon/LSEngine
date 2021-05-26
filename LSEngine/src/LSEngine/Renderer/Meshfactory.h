#pragma once

#include "LSEngine/Core/Core.h"
#include "LSEngine/Renderer/Mesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace LSE {

	class LSE_API MeshFactory
	{
	public:
		static Ref<Mesh> cubeSphere(const float& radius, const int& detail, glm::vec4 colour = { 1.f, 1.f, 1.f, 1.f }, std::vector<int>* strides = nullptr);
		static Ref<Mesh> cubeEllipsoid(const float& a, const float& b, const float& c, const int& detail, glm::vec4 colour = { 1.f, 1.f, 1.f, 1.f }, std::vector<int>* strides = nullptr);

		static Ref<Mesh> sphere(const float& radius, const int& detail, glm::vec4 colour = { 1.f, 1.f, 1.f, 1.f });
		static Ref<Mesh> sphere(const float& radius, const int& w, const int& h, glm::vec4 colour = {1.f, 1.f, 1.f, 1.f});

		static Ref<Mesh> cubeCorner(float size, glm::vec4 colour = { 1.f, 1.f, 1.f, 1.f });
		static Ref<Mesh> cubeCenter(float size, glm::vec4 colour = { 1.f, 1.f, 1.f, 1.f });
		static Ref<Mesh> griddedCubeCenter(float size, int detail, glm::vec4 colour = { 1.f, 1.f, 1.f, 1.f }, std::vector<int>* strides = nullptr);

		static Ref<Mesh> rectCorner(float width, float height, float depth, glm::vec4 colour = { 1.f, 1.f, 1.f, 1.f });
		static Ref<Mesh> colourRectCorner(float width, float height, float depth);

		static Ref<Mesh> rectCenter(float width, float height, float depth, glm::vec4 colour = { 1.f, 1.f, 1.f, 1.f });
		static Ref<Mesh> griddedRectCenter(float width, float height, float depth, int wdetail, int hdetail, int ddetail, glm::vec4 colour = { 1.f, 1.f, 1.f, 1.f }, std::vector<int>* strides = nullptr);
		static Ref<Mesh> colourRectCenter(float width, float height, float depth);

		static Ref<Mesh> planeCenter(glm::vec3 x, glm::vec3 y);
		static Ref<Mesh> planeCorner(glm::vec3 x, glm::vec3 y);

		static Ref<Mesh> gridCenter(glm::vec3 xdir, glm::vec3 y, int xdetail, int ydetail, glm::vec4 colour = { 1.f, 1.f, 1.f, 1.f });
		static Ref<Mesh> gridCorner(glm::vec3 xdir, glm::vec3 y, int xdetail, int ydetail, glm::vec4 colour = { 1.f, 1.f, 1.f, 1.f });

		static Ref<Mesh> rectCenterSharedVertices(float width, float height, float depth);

		typedef glm::vec3(*parametricfunc_t)(const float& t);
		typedef glm::vec4(*parametriccolourfunc_t)(const float& t);
		static Ref<Mesh> paramatric(parametricfunc_t curvefunc, float tstart = 0.f, float tend = 1.f, const int tsteps = 10, parametriccolourfunc_t curvecolourfunc = [](const float& t) { return glm::vec4(1.f, 1.f, 1.f, 1.f); });
		
		static Ref<Mesh> mark();
		static Ref<Mesh> line(const glm::vec3& start, const glm::vec3& end);

		typedef glm::vec3(*surfacefunc_t)(const float& u, const float& v);
		typedef glm::vec4(*surfacecolourfunc_t)(const float& u, const float& v);
		static Ref<Mesh> paramatricSurface(surfacefunc_t curvefunc, float ustart = 0.f, float uend = 1.f, const int usteps = 10, float vstart = 0.f, float vend = 1.f, const int vsteps = 10, surfacecolourfunc_t curvecolourfunc = [](const float& u, const float& v) { return glm::vec4(1.f, 1.f, 1.f, 1.f); });
	};
}