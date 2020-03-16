#pragma once

#include "Camera.h"

namespace LSE {

	class LSE_API PerspectiveCamera : public Camera3D
	{
	public:
		PerspectiveCamera(glm::vec3 pos, glm::vec3 angles, float fov = glm::half_pi<float>(), float ar = 16.f / 9.f, float znear = 0.1f, float zfar = 100.f);

		virtual void SetView(const glm::vec3& pos, const glm::vec3& angles) override;
		virtual void MoveView(const glm::vec3& posoffs, const glm::vec3& angoffs) override;
		virtual void MoveLocalView(const glm::vec3& localoffs, const glm::vec3& angoffs) override;

		void SetZoom(float zoom);

		friend class PerspectiveCameraController;
	private:
		float m_FOV;
		float m_Zoom = 1.f;
	};

}