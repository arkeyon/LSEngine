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

#include <gl/GL.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/color_space.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/norm.hpp>

#include <stdio.h>

class ExampleLayer : public LSE::Layer
{
private:
	LSE::Ref<LSE::Shader> m_MandlebrotShader;
	LSE::Ref<LSE::Shader> m_SimpleShader;
	LSE::Ref<LSE::OrthographicCamera> m_Camera;
	//LSE::Ref<LSE::OrthographicCameraController> m_CameraController;
	LSE::Ref<LSE::Model> m_ScreenModel;
	LSE::Ref<LSE::Model> m_CardioidModel;

	bool m_Paused = false;

	float m_FPS = 0.f;
	float m_Frames = 0.f;
	float m_Time = 0.f;

	bool m_Switch = false;

public:

	ExampleLayer()
		: Layer("ExampleLayer")
	{
		using namespace LSE;

		RendererAPI::SetAPI(RendererAPI::API::OpenGL);
		Renderer::Init();

		float AR = LSE::Application::Get().GetWindow().GetWidth() / LSE::Application::Get().GetWindow().GetHeight();

		m_Camera = MakeRef<OrthographicCamera>(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), -1.5f, 1.5f, -1.5f / AR, 1.5f / AR, -1.f, 1.f);
		//m_CameraController = MakeRef<PerspectiveCameraController>(m_Camera);

		m_MandlebrotShader.reset(Shader::Create("assets/shaders/mandlebrot.glsl"));
		m_SimpleShader.reset(Shader::Create("assets/shaders/simpleshader.glsl"));

		m_ScreenModel = MakeRef<LSE::Model>();
		Ref<Mesh> screenmesh = MeshFactory3D::planeCenter(glm::vec3(3.f, 0.f, 0.f), glm::vec3(0.f, 3.f / AR, 0.f));
		m_ScreenModel->AddMesh(screenmesh);

		m_CardioidModel = MakeRef<LSE::Model>();
		MeshFactory2D::parametricfunc_t cardioidfunc = [](const float& t)
		{
			float v = 1.f - cos(t);
			return glm::vec3(cos(t) * v, sin(t) * v, 0.f);
		};

		Ref<Mesh> cardioidmesh = MeshFactory2D::paramatric(cardioidfunc, 0.f, glm::two_pi<float>(), 100, [](const float& t) { return glm::vec4(1.f, 1.f, 1.f, 1.f); });
		m_CardioidModel->AddMesh(cardioidmesh);

		RenderCommand::SetClearColour(glm::vec4(0.7f, 0.7f, 0.7f, 1.f));
		RenderCommand::EnableDepthTest(false);
	}

	void OnUpdate(float delta) override
	{
		using namespace LSE;

		if (!m_Paused)
		{
			//m_CameraController->OnUpdate(delta);
		}

		m_Frames += 1.f;
		m_Time += delta;

		if (m_Time > 1.f)
		{
			m_FPS = m_Frames / m_Time;
			m_Frames = 0.f;
			m_Time = 0.f;
		}

		float AR = LSE::Application::Get().GetWindow().GetWidth() / LSE::Application::Get().GetWindow().GetHeight();

		RenderCommand::Clear();
		Renderer::BeginScene(m_Camera);
		//glm::vec2 centre = glm::translate(glm::mat4(1.f), glm::vec3(-1.f, -1.f, 0.f)) * LSE::Maths::ScreenInverse(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight()) * glm::vec4(Input::GetMouseX(), Input::GetMouseY(), 0.f, 0.f);
		glm::vec4 centre = glm::vec4(Input::GetMouseX(), Input::GetMouseY(), 0.f, 0.f) * LSE::Maths::ScreenInverse(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
		centre -= glm::vec4(1.f, 1.f, 0.f, 0.f);
		centre.x *= 1.5;
		centre.y *= 1.5 / AR;
		m_MandlebrotShader->Bind();
		m_MandlebrotShader->SetUniform2f("centre", centre);
		m_MandlebrotShader->SetUniformi("julia", m_Switch);

		LSE_TRACE("{0}\t{1}", centre.x, centre.y);

		Renderer::Submit(m_MandlebrotShader, m_ScreenModel);
		Renderer::EndScene();

		if (m_Switch)
		{
			Renderer::Submit(m_SimpleShader, m_CardioidModel, glm::translate(glm::mat4(1.f), glm::vec3(0.25f, 0.f, 0.f)) * glm::scale(glm::mat4(1.f), glm::vec3(0.5f, 0.5f, 1.f)));
			Renderer::EndScene();
		}


	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Debug");
		ImGui::Text((std::string("FPS: ") + std::to_string(m_FPS)).c_str());
		if (m_Paused)
		{
			ImGui::Checkbox("Julia", &m_Switch);
		}
		ImGui::End();
	}

	void OnEvent(LSE::Event& e) override
	{
		using namespace LSE;

		if (!m_Paused)
		{
			//m_CameraController->OnEvent(e);
		}

		EventDispatcher kped(e);
		kped.Dispatch<KeyPressedEvent>([&](KeyPressedEvent& e)
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
	}
};

class Sandbox : public LSE::Application
{
public:
	Sandbox()
		: Application(1280, 720)
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

