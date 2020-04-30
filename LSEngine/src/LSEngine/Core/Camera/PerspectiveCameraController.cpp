#include "lsepch.h"
#include "PerspectiveCameraController.h"

#include "LSEngine/Events/KeyEvent.h"
#include "LSEngine/Events/MouseEvent.h"
#include "LSEngine/Core/KeyMouseCodes.h"
#include "LSEngine/Core/Input.h"

namespace LSE {

	PerspectiveCameraController::PerspectiveCameraController(Ref<PerspectiveCamera>& camera)
		: m_Camera(camera)
	{

	}

	void PerspectiveCameraController::OnEvent(Event& e)
	{
		if (e.GetEventType() == EventType::MouseMoved)
		{
			MouseMovedEvent& me = (MouseMovedEvent&)e;
			m_AngleDirection -= glm::vec3(me.GetDY(), me.GetDX(), 0.f);
		}
	}

	void PerspectiveCameraController::OnUpdate(const float& delta)
	{

		glm::vec3 movedirection(
			Input::IsKeyPressed(LSE_KEY_D) - Input::IsKeyPressed(LSE_KEY_A),
			Input::IsKeyPressed(LSE_KEY_SPACE) - Input::IsKeyPressed(LSE_KEY_LEFT_SHIFT),
			Input::IsKeyPressed(LSE_KEY_W) - Input::IsKeyPressed(LSE_KEY_S)
		);

		m_Camera->MoveLocalView(movedirection * m_MoveSpeed * delta, m_AngleDirection * m_Sensitivity * delta);
		m_AngleDirection = glm::vec3();
	}
}