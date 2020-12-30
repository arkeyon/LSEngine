#include "lsepch.h"
#include "OrthographicCamera.h"

#include "LSEngine/Maths/Vector.h"

namespace LSE {
	
	OrthographicCamera::OrthographicCamera(glm::vec3 pos, glm::vec3 angles, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
		: Camera3D(pos, angles, 1.f, zmin, zmax), m_XMin(xmin), m_XMax(xmax), m_YMin(ymin), m_YMax(ymax)
	{
		m_ProjectionMatrix = glm::ortho(m_XMin, m_XMax, m_YMin, m_YMax, m_ZMin, m_ZMax);
	}

	void OrthographicCamera::SetView(const glm::vec3& pos, const glm::vec3& angles)
	{
		m_Pos = pos;
		m_Angles = angles;
		Maths::NormalizeAngles(m_Angles);
	}

	void OrthographicCamera::MoveView(const glm::vec3& posoffs, const glm::vec3& angoffs)
	{
		m_Pos += posoffs;
		m_Angles += angoffs;
		Maths::NormalizeAngles(m_Angles);
	}

	void OrthographicCamera::MoveLocalView(const glm::vec3& localoffs, const glm::vec3& angoffs)
	{
		m_Angles += angoffs;
		glm::vec3 forward, side, up;
		Maths::AngleVectors(m_Angles, &forward, &side, &up);
		
		m_Pos += localoffs.z * forward + localoffs.x * side + localoffs.y * up;
	}

	const glm::mat4 OrthographicCamera::GetViewMatrix() const
	{
		return glm::mat4(1.f);
	}
	
}