#pragma once

#include "LSEngine/Core.h"
#include <glm/glm.hpp>

namespace LSE {

	class LSE_API Camera3D
	{
	public:
		Camera3D(glm::vec3 pos, glm::vec3 angles, float fov = glm::half_pi<float>(), float ar = 16.f / 9.f, float znear = 0.1f, float zfar = 100.f);
	private:
		float m_FOV;
		float m_AR;
		float m_ZNear, m_ZFar;

		float m_Zoom = 1.f;

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
	};

}