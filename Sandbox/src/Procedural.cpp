
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

#include "LSEngine/Renderer/PlanetFactory.h"

#include <list>

#include "LSEngine/ECS/Objects/ECS.h"

#define BIT(x) (1L << x)

class ExampleLayer : public LSE::Layer
{
private:
	LSE::Ref<LSE::Shader> m_Shader;
	LSE::Ref<LSE::PerspectiveCamera> m_Camera;
	LSE::Ref<LSE::PerspectiveCameraController> m_CameraController;

	bool m_Paused = false;

	LSE::Ref<LSE::Texture2D> m_TestTexture;

	float m_FPS = 0.f;
	float m_Frames = 0.f;
	float m_Time = 0.f;

	LSE::Ref<WorldEntity> m_Skybox;
	LSE::Ref<WorldEntity> m_Sphere;

public:
	ExampleLayer()
		: Layer("ExampleLayer")
	{
		using namespace LSE;

		RendererAPI::SetAPI(RendererAPI::API::OpenGL);
		Renderer::Init();

		m_Camera = MakeRef<PerspectiveCamera>(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::two_pi<float>() / 6.f, 16.f / 9.f, 0.1f, 10000.f);
		m_CameraController = MakeRef<PerspectiveCameraController>(m_Camera);

		m_Shader.reset(Shader::Create("assets/shaders/simpleshader.glsl"));

		LSE::Ref<LSE::Model> skybox = MakeRef<Model>();
		LSE::Ref<LSE::Model> sphere = MakeRef<Model>();

		{
			Ref<Mesh> skyboxmesh = MeshFactory::rectCorner(200.f, 200.f, 200.f);
			skyboxmesh->Invert();
			skybox->AddMesh(skyboxmesh);
		}

		m_Skybox = ECS->CreateEntity<WorldEntity>(glm::vec3(0.f, 0.f, 0.f), glm::angleAxis(0.f, glm::vec3(1.f, 0.f, 0.f)), glm::vec3(1.f, 1.f, 1.f), skybox);
		m_Sphere = ECS->CreateEntity<WorldEntity>(glm::vec3(100.f, 100.f, 100.f), glm::angleAxis(0.f, glm::vec3(1.f, 0.f, 0.f)), glm::vec3(1.f, 1.f, 1.f), PlanetFactory::Planet(80.f));

		m_TestTexture = Texture2D::Create("assets/textures/BLANK.png");

		auto& window = Application::Get().GetWindow();
		window.SetCursorState(false);

		RenderCommand::EnableFaceCulling(true);
		RenderCommand::EnableDepthTest(true);
		//RenderCommand::EnabledWireframe(true);
	}

	void OnUpdate(float delta) override
	{
		using namespace LSE;

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

		//TODO: Put in renderer
		
		m_Shader->SetUniformi("tex", 0);
		m_TestTexture->Bind(0);

		Renderer::BeginScene(m_Camera);

		//Renderer::Submit(m_Shader, m_Skybox->GetComponent<Renderable>()->m_Model);
		//RenderCommand::EnabledWireframe(true);
		Renderer::Submit(m_Shader, m_Sphere->GetComponent<Renderable>()->m_Model, m_Sphere->GetComponent<ReferenceFrame>()->getModelMat());
		//RenderCommand::EnabledWireframe(false);
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Debug");
		ImGui::Text((std::string("FPS: ") + std::to_string(m_FPS)).c_str());
		ImGui::Text((std::string("Camera->Pos: ") + std::to_string(m_Camera->GetPos().x) + ", " + std::to_string(m_Camera->GetPos().y) + ", " + std::to_string(m_Camera->GetPos().z)).c_str());
		ImGui::Text((std::string("Camera->Angles: ") + std::to_string(m_Camera->GetAngles().x) + ", " + std::to_string(m_Camera->GetAngles().y) + ", " + std::to_string(m_Camera->GetAngles().z)).c_str());
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
