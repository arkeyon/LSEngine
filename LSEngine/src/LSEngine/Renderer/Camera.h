#pragma once

#include "LSEngine/Core.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace LSE {

	class LSE_API Camera3D
	{
	public:
		Camera3D(glm::vec3 pos, glm::vec3 angles, float fov = glm::half_pi<float>(), float ar = 16.f / 9.f, float znear = 0.1f, float zfar = 100.f);

		void SetView(const glm::vec3& pos, const glm::vec3& angles);
		void MoveView(const glm::vec3& posoffs, const glm::vec3& angoffs);

		static void NormalizeAngles(glm::vec3& angles);
		static glm::mat4 FPViewMatrix(const glm::vec3& pos, const glm::vec3& angles);
	private:
		float m_FOV;
		float m_AR;
		float m_ZNear, m_ZFar;

		float m_Zoom = 1.f;

		glm::vec3 m_Pos;
		glm::vec3 m_Angles;

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
	};

}