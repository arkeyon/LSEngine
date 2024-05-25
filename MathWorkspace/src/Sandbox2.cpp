
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
#include <complex.h>

#include <list>
#include <math.h>

#include "LSEngine/ECS/Entity.h"
#include "LSEngine/ECS/Objects/ECS.h"

#include "LSEngine/Maths/LSEMath.h"

#include "LSEngine/Maths/Curve.h"

#include <vector>

class ExampleLayer : public LSE::Layer
{
private:
	LSE::Ref<LSE::Model> m_Grid1;
	LSE::Ref<LSE::Model> m_Grid2;

	LSE::Ref<LSE::Shader> m_Shader;
	LSE::Ref<LSE::OrthographicCamera> m_Camera;
	LSE::Ref<LSE::OrthographicCameraController> m_CameraController;

	bool m_Paused = false;

	std::vector<glm::vec3> m_MouseRecord;
	std::array<std::complex<float>, 1000> m_Harmonics;

	glm::vec2 m_LastMousePos;

	LSE::Ref<LSE::Texture2D> m_TestTexture;

	float a = 1.f, b = 0.f;
	float c = 0.f, d = 0.f;

	float m_FPS = 0.f;
	float m_Frames = 0.f;
	float m_Time = 0.f;

public:
	ExampleLayer()
		: Layer("ExampleLayer")
	{
		using namespace LSE;
		using namespace LSE::Maths;

		RendererAPI::SetAPI(RendererAPI::API::OpenGL);
		Renderer::Init();

		m_Camera = MakeRef<OrthographicCamera>(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), -1.f, 1.f, -1.f, 1.f, -1.f, 1.f);
		m_CameraController = MakeRef<OrthographicCameraController>(m_Camera);

		m_Shader.reset(Shader::Create("assets/shaders/complexshader.glsl"));

		m_Grid1 = MakeRef<Model>();
		m_Grid2 = MakeRef<Model>();

		auto func1 = [](const float& u, const float& v)
		{
			return glm::vec3(u, v, 0.f);
		};

		auto cfunc = [](const float& u, const float& v)
		{
			return fmodf((round(fabsf(u) * 10.f) + round(fabsf(v) * 10.f)), 2.f) >= 1.f ? glm::vec4(1.f, 1.f, 1.f, 1.f) : glm::vec4(0.2f, 0.2f, 0.2f, 1.f);
		};

		m_Grid1->AddMesh(MeshFactory3D::paramatricSurface(func1, -1.f, 1.f, 1000, -1.f, 1.f, 1000, cfunc));

		auto func2 = [](const float& u, const float& v)
		{
			return glm::vec3(u * 2.f, v * 2.f, 0.f);
		};

		m_Grid2->AddMesh(MeshFactory3D::paramatricSurface(func2, -1.f, 1.f, 1000, -1.f, 1.f, 1000, cfunc));

		auto& window = Application::Get().GetWindow();
		window.SetCursorState(false);

		RenderCommand::EnableFaceCulling(false);
		RenderCommand::EnableDepthTest(true);
		RenderCommand::SetClearColour(glm::vec4(0.3f, 0.3f, 0.3f, 1.f));
	}

	void OnUpdate(float delta) override
	{
		using namespace LSE;
		using namespace LSE::Maths;

		if (!m_Paused)
		{
			m_CameraController->OnUpdate(delta);
		}

		m_Frames += 1.f;
		m_Time += delta;

		if (m_Time > 1.f)
		{
			m_FPS = m_Frames / m_Time;
			m_Frames = 0.f;
			m_Time = 0.f;
		}

		RenderCommand::Clear();

		Renderer::BeginScene(m_Camera);

		m_Shader->SetUniform1f("a", a);
		m_Shader->SetUniform1f("b", b);
		m_Shader->SetUniform1f("c", c);
		m_Shader->SetUniform1f("d", d);

		Renderer::Submit(m_Shader, m_Grid1);

		Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Debug");
		ImGui::Text((std::string("FPS: ") + std::to_string(m_FPS)).c_str());
		ImGui::SliderFloat("Re 1", &a, -2.f, 2.f);
		ImGui::SliderFloat("Im 1", &b, -2.f, 2.f);
		ImGui::SliderFloat("Re 2", &c, -2.f, 2.f);
		ImGui::SliderFloat("Im 2", &d, -2.f, 2.f);
		ImGui::End();
	}

	void OnEvent(LSE::Event& e) override
	{
		using namespace LSE;

		if (!m_Paused)
		{
			m_CameraController->OnEvent(e);
		}

		EventDispatcher ked(e);
		ked.Dispatch<KeyPressedEvent>([&](KeyPressedEvent& e)
			{
				if (e.GetKeyCode() == LSE_KEY_ESCAPE)
				{
					m_Paused = !m_Paused;
					auto& window = Application::Get().GetWindow();
					if (m_Paused) window.SetCursorState(true);
					else window.SetCursorState(false);
				}
				return false;
			});

		EventDispatcher mped(e);
		mped.Dispatch<MouseButtonPressedEvent>([&](MouseButtonPressedEvent& e)
			{
				if (e.GetButtonCode() == LSE_MOUSE_BUTTON_1)
				{

				}
				return false;
			});

		EventDispatcher mred(e);
		mred.Dispatch<MouseButtonReleasedEvent>([&](MouseButtonReleasedEvent& e)
			{
				if (e.GetButtonCode() == LSE_MOUSE_BUTTON_1)
				{

				}
				return false;
			});
	}
};

class Sandbox : public LSE::Application
{
public:
	Sandbox()
		: Application(1920, 1080)
	{
		PushLayer(MakeScope<ExampleLayer>());
	}

	~Sandbox()
	{

	}
};

LSE::Application* LSE::CreateApplication()
{
	return new Sandbox();
}
