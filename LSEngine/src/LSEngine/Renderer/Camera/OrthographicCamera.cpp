#include "lsepch.h"
#include "OrthographicCamera.h"

namespace LSE {

	OrthoCamera3D::OrthoCamera3D(glm::vec3 pos, glm::vec3 angles, float ar, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
		: Camera3D(pos, angles, ar, zmin, zmax), m_XMin(xmin), m_XMax(xmax), m_YMin(ymin), m_YMax(ymax)
	{
		m_ProjectionMatrix = glm::ortho(m_XMin, m_XMax, m_YMin, m_YMax, m_ZMin, m_ZMax);
		m_ViewMatrix = FPViewMatrix(m_Pos, m_Angles, &m_Forward, &m_Side, &m_Up);
	}

	void OrthoCamera3D::SetView(const glm::vec3& pos, const glm::vec3& angles)
	{
		m_Pos = pos;
		m_Angles = angles;
		NormalizeAngles(m_Angles);

		m_ViewMatrix = FPViewMatrix(m_Pos, m_Angles, &m_Forward, &m_Side, &m_Up);
	}

	void OrthoCamera3D::MoveView(const glm::vec3& posoffs, const glm::vec3& angoffs)
	{
		m_Pos += posoffs;
		m_Angles += angoffs;
		NormalizeAngles(m_Angles);

		m_ViewMatrix = FPViewMatrix(m_Pos, m_Angles, &m_Forward, &m_Side, &m_Up);
	}

	void OrthoCamera3D::MoveLocalView(const glm::vec3& localoffs, const glm::vec3& angoffs)
	{
		m_Angles += angoffs;
		AngleVectors(m_Angles, &m_Forward, &m_Side, &m_Up);

		m_Pos += localoffs.z * m_Forward + localoffs.x * m_Side + localoffs.y * m_Up;
		m_ViewMatrix = FPViewMatrix(m_Pos, m_Forward, m_Side, m_Up);
	}
}