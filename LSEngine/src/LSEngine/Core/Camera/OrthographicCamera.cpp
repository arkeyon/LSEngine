#include "lsepch.h"
#include "OrthographicCamera.h"

#include "LSEngine/Maths/Vector.h"

namespace LSE {

	OrthoCamera3D::OrthoCamera3D(glm::vec3 pos, glm::vec3 angles, float ar, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
		: Camera3D(pos, angles, ar, zmin, zmax), m_XMin(xmin), m_XMax(xmax), m_YMin(ymin), m_YMax(ymax)
	{
		m_ProjectionMatrix = glm::ortho(m_XMin, m_XMax, m_YMin, m_YMax, m_ZMin, m_ZMax);
	}

	void OrthoCamera3D::SetView(const glm::vec3& pos, const glm::vec3& angles)
	{
		m_Pos = pos;
		m_Angles = angles;
		Maths::NormalizeAngles(m_Angles);
	}

	void OrthoCamera3D::MoveView(const glm::vec3& posoffs, const glm::vec3& angoffs)
	{
		m_Pos += posoffs;
		m_Angles += angoffs;
		Maths::NormalizeAngles(m_Angles);
	}

	void OrthoCamera3D::MoveLocalView(const glm::vec3& localoffs, const glm::vec3& angoffs)
	{
		m_Angles += angoffs;
		glm::vec3 forward, side, up;
		Maths::AngleVectors(m_Angles, &forward, &side, &up);
		
		m_Pos += localoffs.z * forward + localoffs.x * side + localoffs.y * up;
	}

}