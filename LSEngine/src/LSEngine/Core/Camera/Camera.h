#pragma once

#include "LSEngine/Core/Core.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "LSEngine/Events/Event.h"

#include "LSEngine/Maths/Vector.h"

namespace LSE {

	class LSE_API Camera3D
	{
	public:
		Camera3D(glm::vec3 pos, glm::vec3 angles, float ar = 16.f / 9.f, float zmin = 0.1f, float zmax = 100.f)
			: m_Pos(pos), m_Angles(angles), m_AR(ar), m_ZMin(zmin), m_ZMax(zmax)
		{}

		virtual ~Camera3D() = default;

		inline const glm::vec3& GetPos() const { return m_Pos; }
		inline const glm::vec3& GetAngles() const { return m_Angles; }
		inline const glm::vec3 GetDir() const
		{
			glm::vec3 forward;
			Maths::AngleVectors(m_Angles, &forward);
			return forward;
		}

		virtual void SetView(const glm::vec3& pos, const glm::vec3& angles) = 0;
		virtual void MoveView(const glm::vec3& posoffs, const glm::vec3& angoffs) = 0;
		virtual void MoveLocalView(const glm::vec3& localoffs, const glm::vec3& angoffs) = 0;
		virtual const glm::mat4 GetViewMatrix() const = 0;

		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline glm::mat4 GetVP() const { return m_ProjectionMatrix * GetViewMatrix(); }
	protected:
		float m_AR;
		float m_ZMin, m_ZMax;

		glm::vec3 m_Angles;
		//glm::vec3 m_Forward, m_Side, m_Up;

		glm::vec3 m_Pos;

		glm::mat4 m_ProjectionMatrix;
	};

}