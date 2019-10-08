#pragma once

#include "LSEngine/Core.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace LSE {

	class LSE_API Camera3D
	{
	public:
		Camera3D(glm::vec3 pos, glm::vec3 angles, float ar = 16.f / 9.f, float zmin = 0.1f, float zmax = 100.f);

		inline const glm::vec3& GetPos() { return m_Pos; }
		inline const glm::vec3& GetAngles() { return m_Angles; }

		virtual void SetView(const glm::vec3& pos, const glm::vec3& angles) = 0;
		virtual void MoveView(const glm::vec3& posoffs, const glm::vec3& angoffs) = 0;
		virtual void MoveLocalView(const glm::vec3& localoffs, const glm::vec3& angoffs) = 0;

		inline const glm::mat4& GetViewMatrix() { return m_ViewMatrix; }
		inline const glm::mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }
		inline glm::mat4 GetVP() { return m_ProjectionMatrix * m_ViewMatrix; }

		static glm::mat4 FPViewMatrix(const glm::vec3& pos, const glm::vec3& angles, glm::vec3* forward = nullptr, glm::vec3* side = nullptr, glm::vec3* up = nullptr);
		static glm::mat4 FPViewMatrix(const glm::vec3& pos, const glm::vec3& forward, const glm::vec3& side, const glm::vec3& up);
		static void AngleVectors(const glm::vec3& angles, glm::vec3* forward = nullptr, glm::vec3* side = nullptr, glm::vec3* up = nullptr);
		static void NormalizeAngles(glm::vec3& angles);
	protected:
		float m_AR;
		float m_ZMin, m_ZMax;

		glm::vec3 m_Angles;
		glm::vec3 m_Forward, m_Side, m_Up;

		glm::vec3 m_Pos;

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
	};

}