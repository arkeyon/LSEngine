#include "lsepch.h"

#include "OrthographicCameraController.h"

#include "LSEngine/Events/KeyEvent.h"
#include "LSEngine/Events/MouseEvent.h"
#include "LSEngine/Core/KeyMouseCodes.h"
#include "LSEngine/Core/Input.h"

namespace LSE {

	OrthographicCameraController::OrthographicCameraController(Ref<OrthographicCamera>& camera)
		: m_Camera(camera)
	{

	}

	void OrthographicCameraController::OnEvent(Event& e)
	{

	}

	void OrthographicCameraController::OnUpdate(const float& delta)
	{

		glm::vec3 movedirection(
			Input::IsKeyPressed(LSE_KEY_D) - Input::IsKeyPressed(LSE_KEY_A),
			Input::IsKeyPressed(LSE_KEY_SPACE) - Input::IsKeyPressed(LSE_KEY_LEFT_SHIFT),
			Input::IsKeyPressed(LSE_KEY_W) - Input::IsKeyPressed(LSE_KEY_S)
		);

		m_AngleDirection += glm::vec3(0.f, 0.f, (float)Input::IsKeyPressed(LSE_KEY_E) - (float)Input::IsKeyPressed(LSE_KEY_Q));

		m_Camera->MoveLocalView(movedirection * m_MoveSpeed * delta, m_AngleDirection * m_Sensitivity * delta);
		m_AngleDirection = glm::vec3();
	}
}