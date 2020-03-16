#include "lsepch.h"
#include "PerspectiveCamera.h"

#include "LSEngine/Maths/Vector.h"

namespace LSE {

	PerspectiveCamera::PerspectiveCamera(glm::vec3 pos, glm::vec3 angles, float fov, float ar, float znear, float zfar)
		: Camera3D(pos, angles, ar, znear, zfar), m_FOV(fov)
	{
		m_ProjectionMatrix = glm::perspective(m_FOV, m_AR, m_ZMin, m_ZMax);
		m_ViewMatrix = Maths::FPViewMatrix(m_Pos, m_Angles, &m_Forward, &m_Side, &m_Up);
	}

	void PerspectiveCamera::SetView(const glm::vec3& pos, const glm::vec3& angles)
	{
		m_Pos = pos;
		m_Angles = angles;
		Maths::NormalizeAngles(m_Angles);

		m_ViewMatrix = Maths::FPViewMatrix(m_Pos, m_Angles);
	}

	void PerspectiveCamera::MoveView(const glm::vec3& posoffs, const glm::vec3& angoffs)
	{
		m_Pos += posoffs;
		m_Angles += angoffs;
		Maths::NormalizeAngles(m_Angles);

		m_ViewMatrix = Maths::FPViewMatrix(m_Pos, m_Angles);
	}

	void PerspectiveCamera::MoveLocalView(const glm::vec3& localoffs, const glm::vec3& angoffs)
	{
		m_Angles += angoffs;
		Maths::AngleVectors(m_Angles, &m_Forward, &m_Side, &m_Up);

		m_Pos += localoffs.z * m_Forward + localoffs.x * m_Side + localoffs.y * m_Up;
		m_ViewMatrix = Maths::FPViewMatrix(m_Pos, m_Forward, m_Side, m_Up);
	}

	void PerspectiveCamera::SetZoom(float zoom)
	{
		m_Zoom = zoom;
		m_ProjectionMatrix = glm::perspective(m_FOV / m_Zoom, m_AR, m_ZMin, m_ZMax);
	}
}