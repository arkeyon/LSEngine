
#include <LSEngine.h>

#include "imgui.h"
#include "imgui_internal.h"

#include "LSEngine/Core/Core.h"
#include "LSEngine/Core/IOUtils.h"
#include "LSEngine/Renderer/VertexArray.h"
#include "LSEngine/Renderer/Meshfactory.h"
#include "LSEngine/Renderer/Renderer.h"
#include "LSEngine/Core/Camera/PerspectiveCameraController.h"
#include "LSEngine/Core/Camera/OrthographicCamera.h"
#include "LSEngine/Renderer/Shader.h"
#include "LSEngine/Renderer/Texture.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/color_space.hpp>
#include <glm/gtc/quaternion.hpp>

#include <list>

#include "LSEngine/ECS/Entity.h"
#include "LSEngine/ECS/Objects/ECS.h"

#define BIT(x) (1L << x)

class ExampleLayer : public LSE::Layer
{
private:
	LSE::Ref<LSE::Model>m_Surface;

	LSE::Ref<LSE::Shader> m_Shader;
	LSE::Ref<LSE::PerspectiveCamera> m_Camera;
	LSE::Ref<LSE::PerspectiveCameraController> m_CameraController;

	bool m_Paused = false;

	LSE::Ref<LSE::Texture2D> m_TestTexture;

	float m_FPS = 0.f;
	float m_Frames = 0.f;
	float m_Time = 0.f;

	float m_Phase = 0.f;
	float m_Speed = 16.f;
	float m_Frequency = 2.f;
	float m_Spacing = 30.f;
	float m_Amplitude = 100.f;
	int m_Dipoles = 4;

	glm::vec4 m_Color = glm::vec4(1.f, 1.f, 1.f, 1.f);
	glm::vec3 m_AmbientColor = 1.f * glm::vec3(1.f, 1.f, 1.f);
	glm::vec3 m_DiffuseColor = 1.f * glm::vec3(1.f, 1.f, 1.f);
	glm::vec3 m_SpecularColor = 1.f * glm::vec3(1.f, 1.f, 1.f);
	float m_Shininess = 10.f;

public:
	ExampleLayer()
		: Layer("ExampleLayer")
	{
		using namespace LSE;

		RendererAPI::SetAPI(RendererAPI::API::OpenGL);
		Renderer::Init();

		m_Camera = MakeRef<PerspectiveCamera>(glm::vec3(-25.f, -60.f, 100.f), glm::vec3(0.71f, 1.497f, 0.f), glm::two_pi<float>() / 6.f, 16.f / 9.f, 0.1f, 10000.f);
		m_CameraController = MakeRef<PerspectiveCameraController>(m_Camera);

		m_Shader.reset(Shader::Create("assets/shaders/waves.glsl"));

		//RenderCommand::EnabledWireframe(true);

		m_Surface = MakeRef<LSE::Model>();

		{
			MeshFactory3D::surfacefunc_t surface = [](const float& u, const float& v)
			{
				return glm::vec3(u, v, 0.f);
			};
		
			MeshFactory3D::surfacecolourfunc_t colorsurface = [](const float& u, const float& v)
			{
				return glm::vec4(0.1f, 0.4f, 0.7f, 1.f);
			};
		
			Ref<Mesh> mesh = MeshFactory3D::paramatricSurface(surface, -300.f, 300.f, 3000, -300.f, +300.f, 3000, colorsurface);
			//mesh->Invert();
			m_Surface->AddMesh(mesh);
		}

		m_TestTexture = Texture2D::Create("assets/textures/BLANK.png");

		auto& window = Application::Get().GetWindow();
		window.SetCursorState(false);

		RenderCommand::EnableFaceCulling(false);
		RenderCommand::EnableDepthTest(true);
	}

	float runtime = 0.f;

	void OnUpdate(float delta) override
	{
		using namespace LSE;

		runtime += delta;

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

		m_Shader->SetUniform1f("u_Time", runtime);
		m_Shader->SetUniform1f("u_Phase", m_Phase);
		m_Shader->SetUniform1f("u_Frequency", m_Frequency);
		m_Shader->SetUniform1f("u_Speed", m_Speed);
		m_Shader->SetUniform1f("u_Spacing", m_Spacing);
		m_Shader->SetUniform1f("u_Amplitude", m_Amplitude);
		m_Shader->SetUniformi("u_Dipoles", m_Dipoles);

		m_Shader->SetUniformi("tex", 0);
		m_TestTexture->Bind(0);

		Renderer::BeginScene(m_Camera);

		Renderer::Submit(m_Shader, m_Surface);

		Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Debug");
		ImGui::Text((std::string("FPS: ") + std::to_string(m_FPS)).c_str());
		ImGui::Text((std::string("Camera->Pos: ") + std::to_string(m_Camera->GetPos().x) + ", " + std::to_string(m_Camera->GetPos().y) + ", " + std::to_string(m_Camera->GetPos().z)).c_str());
		ImGui::Text((std::string("Camera->Angles: ") + std::to_string(m_Camera->GetAngles().x) + ", " + std::to_string(m_Camera->GetAngles().y) + ", " + std::to_string(m_Camera->GetAngles().z)).c_str());
		if (m_Paused)
		{
			ImGui::Spacing();
			ImGui::SliderFloat("Phase",&m_Phase, -15.f, 15.f, "%.3f", 3.f);
			ImGui::SliderFloat("Frequency",&m_Frequency, 1.f, 100.f, "%.3f", 3.f);
			ImGui::SliderFloat("Speed",&m_Speed, 1.f, 100.f, "%.3f", 3.f);
			ImGui::SliderFloat("Spacing",&m_Spacing, 0.f, 200.f, "%.3f", 3.f);
			ImGui::SliderFloat("Amplitude",&m_Amplitude, 1.f, 100.f, "%.3f", 3.f);
			ImGui::SliderInt("Dipoles",&m_Dipoles, 0, 100);
		}
		ImGui::End();
	}

	void OnEvent(LSE::Event& e) override
	{
		using namespace LSE;

		if (!m_Paused)
		{
			m_CameraController->OnEvent(e);
		}

		EventDispatcher ed(e);
		ed.Dispatch<KeyPressedEvent>([&](KeyPressedEvent& e)
			{
				if (((KeyPressedEvent&)e).GetKeyCode() == LSE_KEY_ESCAPE)
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
