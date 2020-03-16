#include <LSEngine.h>

#include "imgui.h"

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

glm::mat3 star(glm::vec3& v)
{
	glm::mat3 result;
	result[0][0] = 0.f;
	result[1][0] = v.z;
	result[2][0] = -v.y;
	result[0][1] = -v.z;
	result[1][1] = 0.f;
	result[2][1] = v.x;
	result[0][2] = v.y;
	result[1][2] = -v.x;
	result[2][2] = 0.f;

	return result;
}

class ExampleLayer : public LSE::Layer
{
private:
	LSE::Ref<LSE::Model> m_Skybox, m_Cube;
	LSE::Ref<LSE::Shader> m_Shader;
	LSE::Ref<LSE::PerspectiveCamera> m_Camera;
	LSE::Ref<LSE::PerspectiveCameraController> m_CameraController;

	bool m_Paused = false;

	LSE::Ref<LSE::Texture2D> m_TestTexture;

	float m_FPS = 0.f;
	float m_Frames = 0.f;
	float m_Time = 0.f;

	glm::vec4 m_Color = glm::vec4(1.f, 1.f, 1.f, 1.f);
	glm::vec3 m_AmbientColor	 = 1.f * glm::vec3(1.f, 1.f, 1.f);
	glm::vec3 m_DiffuseColor	 = 1.f * glm::vec3(1.f, 1.f, 1.f);
	glm::vec3 m_SpecularColor	 = 1.f * glm::vec3(1.f, 1.f, 1.f);
	float m_Shininess			 = 10.f;
public:
	ExampleLayer()
		: Layer("ExampleLayer")
	{
		using namespace LSE;

		RendererAPI::SetAPI(RendererAPI::API::OpenGL);
		Renderer::Init();

		m_Camera.reset(new PerspectiveCamera(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::two_pi<float>() / 6.f, 16.f / 9.f, 0.1f, 1000.f));
		m_CameraController.reset(new PerspectiveCameraController(m_Camera));

		m_Shader.reset(Shader::Create("assets/shaders/simpleshader.glsl"));

		m_Skybox = MakeRef<Model>();
		m_Cube = MakeRef<Model>();

		{
			Ref<Mesh> mesh = MeshFactory::generateRectCorner(200.f, 200.f, 200.f);
			mesh->Invert();

			m_Skybox->AddMesh(mesh);
		}

		{
			Ref<Mesh> mesh = MeshFactory::generateCubeCenter(20.f);
			m_Cube->AddMesh(mesh);
		}

		m_TestTexture = Texture2D::Create("assets/textures/TEST.png");

		auto& window = Application::Get().GetWindow();
		window.SetCursorState(false);
	}

	glm::vec3 pos = glm::vec3(20.f, 20.f, 20.f);
	glm::vec3 vel = glm::vec3(0.f, 0.f, 0.f);
	glm::mat3 rotation = glm::rotate(glm::mat4(), 0.f, glm::vec3());
	glm::vec3 omega = glm::vec3(0.f, 0.f, 0.f);
	float mass = 10.f;

	void OnUpdate(float delta) override
	{
		using namespace LSE;

		if (!m_Paused)
		{
			m_CameraController->OnUpdate(delta);
		}



		for (int i = 0; i < m_Cube->m_Meshs[0]->m_VerticesCount; ++i)
		{
			glm::vec3 r = rotation * m_Cube->m_Meshs[0]->m_Vertices[i].a_Position;
			glm::vec3 p = r + pos;

			if (p.z < 0.f)
			{
				float e = 1.f;
				glm::vec3 n(0.f, 0.f, 1.f);
				float j = -(1.f + e) * glm::dot(vel + glm::cross(omega, r), n) / (1.f / mass + );
			}
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

		Renderer::BeginScene(m_Camera);
		Renderer::Submit(m_Shader, m_Skybox);
		Renderer::Submit(m_Shader, m_Cube, glm::translate(glm::mat4(1.f), glm::vec3(15.f, 15.f, 15.f)));
		Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Debug");
		ImGui::Text((std::string("FPS: ") + std::to_string(m_FPS)).c_str());
		ImGui::Text((std::string("Camera->Pos: ") + std::to_string(m_Camera->GetPos().x) + ", " + std::to_string(m_Camera->GetPos().y) + ", " + std::to_string(m_Camera->GetPos().z)).c_str());
		ImGui::Text((std::string("Camera->Angles: ") + std::to_string(m_Camera->GetAngles().x) + ", " + std::to_string(m_Camera->GetAngles().y)).c_str());
		ImGui::Spacing();
		ImGui::ColorEdit4("Color", &m_Color[0]);
		ImGui::ColorEdit3("AmbientColor", &m_AmbientColor[0]);
		ImGui::ColorEdit3("DiffuseColor", &m_DiffuseColor[0]);
		ImGui::ColorEdit3("SpecularColor", &m_SpecularColor[0]);
		ImGui::SliderFloat("Shininess", &m_Shininess, 1.f, 10.f, "%.3f", 3.f);
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
