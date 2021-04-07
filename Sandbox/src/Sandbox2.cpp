
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
#include "ECS.h"

#define BIT(x) (1L << x)

class ExampleLayer : public LSE::Layer
{
private:
	LSE::Ref<LSE::Model> m_Skybox, m_Cube, m_Door;

	LSE::Ref<LSE::Shader> m_Shader;
	LSE::Ref<LSE::PerspectiveCamera> m_Camera;
	LSE::Ref<LSE::PerspectiveCameraController> m_CameraController;

	bool m_Paused = false;

	LSE::Ref<LSE::Texture2D> m_TestTexture;

	float m_FPS = 0.f;
	float m_Frames = 0.f;
	float m_Time = 0.f;

	glm::vec4 m_Color = glm::vec4(1.f, 1.f, 1.f, 1.f);
	glm::vec3 m_AmbientColor = 1.f * glm::vec3(1.f, 1.f, 1.f);
	glm::vec3 m_DiffuseColor = 1.f * glm::vec3(1.f, 1.f, 1.f);
	glm::vec3 m_SpecularColor = 1.f * glm::vec3(1.f, 1.f, 1.f);
	float m_Shininess = 10.f;

	LSE::Ref<WorldEntity> m_Door1;
	LSE::Ref<WorldEntity> m_Door2;
	LSE::Ref<WorldEntity> m_Player;

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

		m_Skybox = MakeRef<Model>();
		m_Cube = MakeRef<Model>();
		m_Door = MakeRef<Model>();

		{
			Ref<Mesh> mesh = MeshFactory::rectCorner(200.f, 200.f, 200.f);
			mesh->Invert();
		
			m_Skybox->AddMesh(mesh);
		}
		
		{
			Ref<Mesh> mesh = MeshFactory::cubeCenter(5.f);
			m_Cube->AddMesh(mesh);
		}
		
		//{
		//	MeshFactory::surfacefunc_t surface = [](const float& u, const float& v)
		//	{
		//		return glm::vec3(u, 10.f * cos(v), 10.f * sin(v));
		//	};
		//
		//	MeshFactory::surfacecolourfunc_t colorsurface = [](const float& u, const float& v)
		//	{
		//		return glm::vec4(glm::rgbColor(glm::vec3(v / glm::two_pi<float>() * 360.f, 1.f, 1.f)), 1.f);
		//	};
		//
		//	Ref<Mesh> mesh = MeshFactory::paramatricSurface(surface, 0.f, 100.f, 5, 0.f, glm::two_pi<float>(), 20, colorsurface);
		//	//mesh->Invert();
		//	m_Door->AddMesh(mesh);
		//}

		m_Door->AddMesh(MeshFactory::planeCorner(glm::vec3(20.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 80.f)));

		m_Door1 = MakeRef<WorldEntity>(glm::vec3(80.f, 80.f, 0.f), glm::angleAxis(0.f, glm::vec3(1.f, 0.f, 0.f)), glm::vec3(1.f, 1.f, 1.f), m_Door);
		m_Door2 = MakeRef<WorldEntity>(glm::vec3(120.f, 120.f, 0.f), glm::angleAxis(glm::quarter_pi<float>(), glm::vec3(1.f, 0.f, 0.f)), glm::vec3(1.f, 1.f, 1.f), m_Door);
		m_Player = MakeRef<WorldEntity>(glm::vec3(50.f, 50.f, 0.f), glm::angleAxis(0.f, glm::vec3(0.f, 0.f, 1.f)), glm::vec3(1.f, 1.f, 1.f), m_Cube);

		m_TestTexture = Texture2D::Create("assets/textures/BLANK.png");

		auto& window = Application::Get().GetWindow();
		window.SetCursorState(false);

		RenderCommand::EnableFaceCulling(false);
		RenderCommand::EnableDepthTest(true);
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

		m_Shader->SetUniform4f("u_Color", m_Color);							  //TODO: Put in renderer
		m_Shader->SetUniform3f("u_AmbientColor", m_AmbientColor);
		m_Shader->SetUniform3f("u_DiffuseColor", m_DiffuseColor);
		m_Shader->SetUniform3f("u_SpecularColor", m_SpecularColor);

		m_Shader->SetUniformi("tex", 0);
		m_TestTexture->Bind(0);

		glm::mat4 im = m_Door1->GetComponent<ReferenceFrame>()->getModelMat() * glm::inverse(m_Door2->GetComponent<ReferenceFrame>()->getModelMat());

		Renderer::BeginScene(m_Camera);
		Renderer::Submit(m_Shader, m_Skybox);
		Renderer::Submit(m_Shader, m_Cube, glm::inverse(m_Camera->GetViewMatrix() * im));
		Renderer::Submit(m_Shader, m_Door, m_Door2->GetComponent<ReferenceFrame>()->getModelMat());

		RenderCommand::EnableStencil(true);
		RenderCommand::StencilDraw(true);
		Renderer::Submit(m_Shader, m_Door, m_Door1->GetComponent<ReferenceFrame>()->getModelMat());
		RenderCommand::StencilDraw(false);
		RenderCommand::Clear(false, true, false);

		Renderer::BeginScene({ m_Camera->GetVP() * im, im * glm::vec4(m_Camera->GetPos(), 1.f), m_Camera->GetDir() });
		Renderer::Submit(m_Shader, m_Skybox);
		Renderer::Submit(m_Shader, m_Door, m_Door1->GetComponent<ReferenceFrame>()->getModelMat());

		RenderCommand::EnableStencil(false);

		//m_Door2->GetComponent<ReferenceFrame>()->m_Orin *= glm::angleAxis(delta * 0.5f, glm::vec3(1.f, 0.f, 0.f));
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
			ImGui::ColorEdit4("Color", &m_Color[0]);
			ImGui::ColorEdit3("AmbientColor", &m_AmbientColor[0]);
			ImGui::ColorEdit3("DiffuseColor", &m_DiffuseColor[0]);
			ImGui::ColorEdit3("SpecularColor", &m_SpecularColor[0]);
			ImGui::SliderFloat("Shininess", &m_Shininess, 1.f, 10.f, "%.3f", 3.f);
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
