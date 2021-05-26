
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

class ExampleLayer : public LSE::Layer
{
private:
	LSE::Ref<LSE::Model> m_ParabolaModel;
	LSE::Ref<LSE::Model> m_LineModel;
	LSE::Ref<LSE::Model> m_Mark;

	LSE::Ref<LSE::Shader> m_Shader;
	LSE::Ref<LSE::OrthographicCamera> m_Camera;
	LSE::Ref<LSE::OrthographicCameraController> m_CameraController;

	bool m_Paused = false;

	LSE::Ref<LSE::Texture2D> m_TestTexture;

	float m_FPS = 0.f;
	float m_Frames = 0.f;
	float m_Time = 0.f;

	LSE::Ref<WorldEntity> m_MeshEntity;

public:
	ExampleLayer()
		: Layer("ExampleLayer")
	{
		using namespace LSE;

		RendererAPI::SetAPI(RendererAPI::API::OpenGL);
		Renderer::Init();

		m_Camera = MakeRef<OrthographicCamera>(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), -10.f, 10.f, -10.f, 10.f, -1.f, 1.f);
		m_CameraController = MakeRef<OrthographicCameraController>(m_Camera);

		m_Shader.reset(Shader::Create("assets/shaders/simpleshader.glsl"));

		m_ParabolaModel = MakeRef<Model>();

		m_LineModel = MakeRef<Model>();
		m_LineModel->AddMesh(MeshFactory::line(glm::vec3(-5.f, 0.f, 0.f), glm::vec3(5.f, 0.f, 0.f)));

		m_Mark = MakeRef<Model>();
		m_Mark->AddMesh(MeshFactory::mark());
		
		{
			MeshFactory::parametricfunc_t func = [](const float& t)
			{
				return glm::vec3(t, t * t, 0.f);
			};
		
			MeshFactory::parametriccolourfunc_t colorfunc = [](const float& t)
			{
				return glm::vec4(1.f, 1.f, 1.f, 1.f);
			};
		
			//Ref<Mesh> mesh = MeshFactory::paramatricSurface(surface, -5.f, 5.f, 400, 0.f, 1.f, 3, colorsurface);
			Ref<Mesh> mesh = MeshFactory::paramatric(func, -10.f, 10.f, 500, colorfunc);

			m_ParabolaModel->AddMesh(mesh);
		}

		m_MeshEntity = MakeRef<WorldEntity>(glm::vec3(0.f, 0.f, 0.f), glm::angleAxis(0.f, glm::vec3(1.f, 0.f, 0.f)), glm::vec3(1.f, 1.f, 1.f), m_ParabolaModel);

		m_TestTexture = Texture2D::Create("assets/textures/BLANK.png");

		auto& window = Application::Get().GetWindow();
		window.SetCursorState(false);

		RenderCommand::EnableFaceCulling(false);
		RenderCommand::EnableDepthTest(true);
		RenderCommand::SetClearColour(glm::vec4(0.3f, 0.3f, 0.3f, 1.f));
	}

	float rot = -glm::half_pi<float>();

	glm::mat4 A = glm::translate(glm::mat4(1.f), glm::vec3(-0.8f, -0.8f, 0.f));
	glm::mat4 B = glm::rotate(glm::mat4(1.f), rot, glm::vec3(0.f, 0.f, 1.f));

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

		m_Shader->SetUniformi("tex", 0);
		m_TestTexture->Bind(0);

		Renderer::BeginScene(m_Camera);

		B = glm::rotate(glm::mat4(1.f), rot += 0.001f, glm::vec3(0.f, 0.f, 1.f));
		
		glm::vec2 t1 = find(glm::vec2(5.f, 5.f));
		glm::vec2 t2 = find(glm::vec2(5.f, -5.f));
		glm::vec2 t3 = find(glm::vec2(-5.f, 5.f));
		glm::vec2 t4 = find(glm::vec2(-5.f, -5.f));

		glm::vec4 T1(t1[0], t1[0] * t1[0], 0.f, 1.f);
		glm::vec4 T2(t2[0], t2[0] * t2[0], 0.f, 1.f);
		glm::vec4 T3(t3[0], t3[0] * t3[0], 0.f, 1.f);
		glm::vec4 T4(t4[0], t4[0] * t4[0], 0.f, 1.f);

		Renderer::Submit(m_Shader, m_Mark, glm::translate(glm::mat4(1.f), glm::vec3(A * T1)));
		Renderer::Submit(m_Shader, m_Mark, glm::translate(glm::mat4(1.f), glm::vec3(A * T2)));
		Renderer::Submit(m_Shader, m_Mark, glm::translate(glm::mat4(1.f), glm::vec3(A * T3)));
		Renderer::Submit(m_Shader, m_Mark, glm::translate(glm::mat4(1.f), glm::vec3(A * T4)));

		Renderer::Submit(m_Shader, m_MeshEntity->GetComponent<Renderable>()->m_Model, glm::mat4(A));
		Renderer::Submit(m_Shader, m_MeshEntity->GetComponent<Renderable>()->m_Model, glm::mat4(B));

		Renderer::EndScene();
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
