#pragma once

#include "LSEngine/Core/Camera/PerspectiveCamera.h"

namespace LSE {

	class PerspectiveCameraController
	{
	public:
		PerspectiveCameraController(Ref<PerspectiveCamera>& camera);

		void OnEvent(Event& e);
		void OnUpdate(const float& delta);
	private:
		Ref<PerspectiveCamera> m_Camera;
		glm::vec3 m_AngleDirection = glm::vec3(0.f, 0.f, 0.f);
		glm::vec3 m_Sensitivity = glm::vec3(6.f, 4.f * 16.f / 9.f, 1.f) / 20.f;
		glm::vec3 m_MoveSpeed = glm::vec3(20.f, 20.f, 20.f);
	};
}