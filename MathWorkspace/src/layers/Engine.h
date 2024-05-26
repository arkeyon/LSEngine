#pragma once

#include <LSEngine.h>

#include "imgui.h"
#include "imgui_internal.h"

#include "LSEngine/Core/Core.h"
#include "LSEngine/Core/IOUtils.h"
#include "LSEngine/Renderer/VertexArray.h"
#include "LSEngine/Renderer/Meshfactory.h"
#include "LSEngine/Renderer/Renderer.h"
#include "LSEngine/Core/Camera/PerspectiveCameraController.h"
#include "LSEngine/Core/Camera/OrthographicCameraController.h"
#include "LSEngine/Core/Camera/OrthographicCamera.h"
#include "LSEngine/Renderer/Shader.h"
#include "LSEngine/Renderer/Texture.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/color_space.hpp>
#include <glm/gtc/quaternion.hpp>

#include <list>
#include <math.h>

#include "LSEngine/ECS/Entity.h"
#include "LSEngine/ECS/Objects/ECS.h"

#include "LSEngine/Maths/LSEMath.h"

#include "LSEngine/Maths/Curve.h"

class EngineLayer : public LSE::Layer
{
private:
	LSE::Ref<LSE::Maths::Parabola> m_Parabola1;
	LSE::Ref<LSE::Maths::Parabola> m_Parabola2;
	LSE::Ref<LSE::Maths::Line> m_Line1;
	LSE::Ref<LSE::Maths::Line> m_Line2;

	LSE::Ref<LSE::Maths::UnnamedCurve> m_UCurve;
	LSE::Ref<LSE::Model> m_Mark;

	LSE::Ref<LSE::Shader> m_Shader;
	LSE::Ref<LSE::OrthographicCamera> m_Camera;
	LSE::Ref<LSE::OrthographicCameraController> m_CameraController;

	bool m_Paused = false;

	LSE::Ref<LSE::Texture2D> m_TestTexture;

	float m_FPS = 0.f;
	float m_Frames = 0.f;
	float m_Time = 0.f;

public:
	EngineLayer();

	void OnUpdate(float delta) override;
	void OnImGuiRender() override;
	void OnEvent(LSE::Event& e) override;
};