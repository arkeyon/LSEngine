#include "lsepch.h"
#include "PerspectiveCamera.h"

#include "LSEngine/Maths/Vector.h"

namespace LSE {

	PerspectiveCamera::PerspectiveCamera(glm::vec3 pos, glm::vec3 angles, float fov, float ar, float znear, float zfar)
		: Camera3D(pos, angles, ar, znear, zfar), m_FOV(fov), m_Zoom(1.f)
	{
		m_ProjectionMatrix = glm::perspective(m_FOV, m_AR, m_ZMin, m_ZMax);
	}

	PerspectiveCamera::PerspectiveCamera(const PerspectiveCamera& camera)
		: Camera3D(camera.m_Pos, camera.m_Angles, camera.m_AR, camera.m_ZMin, camera.m_ZMax), m_FOV(camera.m_FOV), m_Zoom(camera.m_Zoom)
	{
		m_ProjectionMatrix = glm::perspective(m_FOV, m_AR, m_ZMin, m_ZMax);
	}

	void PerspectiveCamera::SetView(const glm::vec3& pos, const glm::vec3& angles)
	{
		m_Pos = pos;
		m_Angles = angles;
		Maths::NormalizeAngles(m_Angles);
	}

	void PerspectiveCamera::MoveView(const glm::vec3& posoffs, const glm::vec3& angoffs)
	{
		m_Pos += posoffs;
		m_Angles += angoffs;
		Maths::NormalizeAngles(m_Angles);
	}

	void PerspectiveCamera::MoveLocalView(const glm::vec3& localoffs, const glm::vec3& angoffs)
	{
		m_Angles += angoffs;
		Maths::NormalizeAngles(m_Angles);

		glm::vec3 forward, side, up;

		Maths::AngleVectors(m_Angles, &forward, &side, &up);

		m_Pos += localoffs.z * forward + localoffs.x * side + localoffs.y * up;
	}

	void PerspectiveCamera::SetZoom(float zoom)
	{
		m_Zoom = zoom;
		m_ProjectionMatrix = glm::perspective(m_FOV / m_Zoom, m_AR, m_ZMin, m_ZMax);
	}
}