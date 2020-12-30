#pragma once

#include "Camera.h"

namespace LSE {
	
	class LSE_API OrthographicCamera : public Camera3D
	{
	public:
		OrthographicCamera(glm::vec3 pos, glm::vec3 angles, float xmin = -50.f, float xmax = 50.f, float ymin = -50.f, float ymax = 50.f, float zmin = -50.f, float zmax = 20.f);

		virtual void SetView(const glm::vec3& pos, const glm::vec3& angles) override;
		virtual void MoveView(const glm::vec3& posoffs, const glm::vec3& angoffs) override;
		virtual void MoveLocalView(const glm::vec3& localoffs, const glm::vec3& angoffs) override;
		virtual const glm::mat4 GetViewMatrix() const override;
	private:
		float m_XMin, m_XMax;
		float m_YMin, m_YMax;
	};
	
}