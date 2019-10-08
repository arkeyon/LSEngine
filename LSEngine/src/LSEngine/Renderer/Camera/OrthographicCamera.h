#pragma once

#include "Camera.h"

namespace LSE {

	class LSE_API OrthoCamera3D : public Camera3D
	{
	public:
		OrthoCamera3D(glm::vec3 pos, glm::vec3 angles, float ar = 16.f / 9.f, float xmin = -20.f, float xmax = 20.f, float ymin = -20.f, float ymax = 20.f, float zmin = -20.f, float zmax = 20.f);

		virtual void SetView(const glm::vec3& pos, const glm::vec3& angles) override;
		virtual void MoveView(const glm::vec3& posoffs, const glm::vec3& angoffs) override;
		virtual void MoveLocalView(const glm::vec3& localoffs, const glm::vec3& angoffs) override;
	private:
		float m_XMin, m_XMax;
		float m_YMin, m_YMax;
	};

}