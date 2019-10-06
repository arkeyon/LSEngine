#include <lsepch.h>
#include "Camera.h"

#include <glm/gtx/transform.hpp>

namespace LSE {

	Camera3D::Camera3D(glm::vec3 pos, glm::vec3 angles, float fov, float ar, float znear, float zfar)
		: m_FOV(fov), m_AR(ar), m_ZNear(znear), m_ZFar(zfar)
	{
		m_ProjectionMatrix = glm::perspective(m_FOV, m_AR, m_ZNear, m_ZFar);
		m_ViewMatrix = FPViewMatrix(m_Pos, m_Angles);
	}

	void Camera3D::SetView(const glm::vec3& pos, const glm::vec3& angles)
	{
		m_Pos = pos;
		m_Angles = angles;
		NormalizeAngles(m_Angles);

		m_ViewMatrix = FPViewMatrix(m_Pos, m_Angles);
	}

	void Camera3D::MoveView(const glm::vec3& posoffs, const glm::vec3& angoffs)
	{
		m_Pos += posoffs;
		m_Angles += angoffs;
		NormalizeAngles(m_Angles);

		m_ViewMatrix = FPViewMatrix(m_Pos, m_Angles);
	}

	void Camera3D::NormalizeAngles(glm::vec3& angles)
	{
		while (angles.y > glm::pi<float>()) angles.y -= glm::pi<float>();
		while (angles.y < -glm::pi<float>()) angles.y += glm::pi<float>();

		if (angles.x > glm::half_pi<float>()) angles.x = glm::half_pi<float>();
		if (angles.x < -glm::half_pi<float>()) angles.x = -glm::half_pi<float>();

		angles.z = 0.f;
	}

	glm::mat4 Camera3D::FPViewMatrix(const glm::vec3& pos, const glm::vec3& angles)
	{
		float cospitch = cosf(angles.x);
		float sinpitch = cosf(angles.x);
		float cosyaw = cosf(angles.y);
		float sinyaw = cosf(angles.y);

		glm::vec3 f(cosyaw * cospitch, sinyaw * cospitch, sinpitch);
		glm::vec3 const s(f.y, -f.x, 0.f);
		glm::vec3 const u(cosyaw * -sinpitch, sinyaw * -sinpitch, cospitch);

		glm::mat4 Result;
		Result[0][0] = s.x;
		Result[1][0] = s.y;
		Result[2][0] = s.z;
		Result[0][1] = u.x;
		Result[1][1] = u.y;
		Result[2][1] = u.z;
		Result[0][2] = f.x;
		Result[1][2] = f.y;
		Result[2][2] = f.z;
		Result[3][0] = -glm::dot(s, pos);
		Result[3][1] = -glm::dot(u, pos);
		Result[3][2] = -glm::dot(f, pos);
		Result[0][3] = 0.f;
		Result[1][3] = 0.f;
		Result[2][3] = 0.f;
		Result[3][3] = 1.f;

		return Result;
	}

}