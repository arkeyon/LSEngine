
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
#include <glm/gtc/quaternion.hpp>

#include <list>

#include "LSEngine/ECS/Entity.h"
#include "LSEngine/ECS/Objects/ECS.h"
#include "LSEngine/Core/EntryPoint.h"


#define BIT(x) (1L << x)

class ExampleLayer : public LSE::Layer
{
private:
	LSE::Ref<LSE::Model> m_Skybox;
	LSE::Ref<LSE::Model> m_Door1;
	LSE::Ref<LSE::Model> m_Door2;
	LSE::Ref<LSE::Model> m_Box1;
	LSE::Ref<LSE::Model> m_Box2;
	LSE::Ref<LSE::Model> m_Box3;


	LSE::Ref<LSE::Model> m_Camerablock;


	glm::mat4 m_Door1transform;
	glm::mat4 m_Door2transform;

	glm::vec3 m_Lightpos = glm::vec3(100.f, 100.f, 180.f);

	LSE::Ref<LSE::Shader> m_Shader;
	LSE::Ref<LSE::Shader> m_PortalShader;
	LSE::Ref<LSE::PerspectiveCamera> m_Camera;
	LSE::Ref<LSE::PerspectiveCameraController> m_CameraController;

	bool m_Paused = false;

	float m_FPS = 0.f;
	float m_Frames = 0.f;
	float m_Time = 0.f;

	glm::vec4 m_Color = glm::vec4(1.f, 1.f, 1.f, 1.f);
	glm::vec3 m_AmbientColor = 1.f * glm::vec3(1.f, 1.f, 1.f);
	glm::vec3 m_DiffuseColor = 1.f * glm::vec3(1.f, 1.f, 1.f);
	glm::vec3 m_SpecularColor = 1.f * glm::vec3(1.f, 1.f, 1.f);
	float m_Shininess = 10.f;

	float angle = 0.f;

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
		m_PortalShader.reset(Shader::Create("assets/shaders/doorshader.glsl"));

		//RenderCommand::EnabledWireframe(true);

		m_Skybox = MakeRef<LSE::Model>();
		{
			auto skybox = MeshFactory3D::colourRectCorner(100.f, 100.f, 100.f);
			skybox->Invert();
			m_Skybox->AddMesh(skybox);
		}

		m_Box1 = MakeRef<LSE::Model>();
		{
			auto skybox = MeshFactory3D::cubeCorner(15.f, glm::vec4(1.f, 0.5f, 0.8f, 1.f));
			m_Box1->AddMesh(skybox);
		}

		m_Box2 = MakeRef<LSE::Model>();
		{
			auto skybox = MeshFactory3D::cubeCorner(15.f, glm::vec4(0.2f, 0.6f, 0.8f, 1.f));
			m_Box2->AddMesh(skybox);
		}

		m_Box3 = MakeRef<LSE::Model>();
		{
			auto skybox = MeshFactory3D::cubeCorner(15.f, glm::vec4(0.5f, 0.2f, 0.2f, 1.f));
			m_Box3->AddMesh(skybox);
		}

		m_Camerablock = MakeRef<LSE::Model>();
		{
			auto skybox = MeshFactory3D::cubeCenter(4.f, glm::vec4(0.8f, 0.8f, 0.2f, 1.f));
			m_Camerablock->AddMesh(skybox);
		}

		m_Door1 = MakeRef<LSE::Model>();
		m_Door1transform = glm::translate(glm::mat4(1.f), glm::vec3(50.f, 50.f, 0.f));
		{
			auto door = MeshFactory2D::elipse(5.f, 10.f, 50);
			door->Transform(glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 10.f)) * glm::rotate(glm::mat4(1.f), glm::half_pi<float>(), glm::vec3(1.f, 0.f, 0.f)));
			m_Door1->AddMesh(door);
		}

		m_Door2 = MakeRef<LSE::Model>();
		m_Door2transform = glm::translate(glm::mat4(1.f), glm::vec3(80.f, 30.f, 0.f)) * glm::rotate(glm::mat4(1.f), glm::half_pi<float>(), glm::vec3(0.f, 0.f, 1.f)) * glm::scale(glm::mat4(1.f), glm::vec3(1.f, 1.f, 5.f));
		{
			auto door = MeshFactory2D::elipse(5.f, 10.f, 50);
			door->Transform(glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 10.f)) * glm::rotate(glm::mat4(1.f), glm::half_pi<float>(), glm::vec3(1.f, 0.f, 0.f)));
			m_Door2->AddMesh(door);
		}

		auto& window = Application::Get().GetWindow();
		window.SetCursorState(false);

		RenderCommand::EnableFaceCulling(true);

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

		//angle += delta * 0.5f;

		if (m_Time > 1.f)
		{
			m_FPS = m_Frames / m_Time;
			m_Frames = 0.f;
			m_Time = 0.f;
		}

		RenderCommand::Clear();

		Renderer::BeginScene(m_Camera);

		RenderCommand::StencilDraw(0);

		Renderer::Submit(m_Shader, m_Door1, m_Door1transform);

		RenderCommand::StencilDraw(1);

		glm::mat4 trans1 = m_Door1transform * glm::inverse(m_Door2transform * glm::rotate(glm::mat4(1.f), angle, glm::vec3(0.f, 0.f, 1.f)));

		m_PortalShader->Bind();
		m_PortalShader->SetUniform3f("lightpos", glm::vec3(trans1 * glm::vec4(m_Lightpos, 1.f)));
		m_PortalShader->SetUniform3f("portalnormal", m_Door1->m_Meshs[0]->m_Vertices[0].a_Normal);
		m_PortalShader->SetUniform3f("portalpos", glm::vec3(m_Door1transform * glm::vec4(m_Door1->m_Meshs[0]->m_Vertices[0].a_Position, 1.f)));


		Renderer::Submit(m_PortalShader, m_Skybox, trans1);
		Renderer::Submit(m_PortalShader, m_Box1, trans1 * glm::translate(glm::mat4(1.f), glm::vec3(20.f, 70.f, 0.f)));
		Renderer::Submit(m_PortalShader, m_Box2, trans1 * glm::translate(glm::mat4(1.f), glm::vec3(70.f, 70.f, 0.f)));
		Renderer::Submit(m_PortalShader, m_Box3, trans1 * glm::translate(glm::mat4(1.f), glm::vec3(20.f, 20.f, 0.f)));

		RenderCommand::StencilDraw(2);

		Renderer::Submit(m_Shader, m_Door1, m_Door1transform);

		RenderCommand::StencilDraw(3);
		//RenderCommand::StencilDraw(0);
		//
		//Renderer::Submit(m_Shader, m_Door2, m_Door2transform);
		//
		//RenderCommand::StencilDraw(1);
		//
		glm::mat4 trans2 = m_Door2transform * glm::rotate(glm::mat4(1.f), angle, glm::vec3(0.f, 0.f, 1.f)) * glm::inverse(m_Door1transform);
		//
		//Renderer::Submit(m_Shader, m_Skybox, trans2);
		//Renderer::Submit(m_Shader, m_Box1, trans2 * glm::translate(glm::mat4(1.f), glm::vec3(20.f, 70.f, 0.f)));
		//Renderer::Submit(m_Shader, m_Box2, trans2 * glm::translate(glm::mat4(1.f), glm::vec3(70.f, 70.f, 0.f)));
		//Renderer::Submit(m_Shader, m_Box3, trans2 * glm::translate(glm::mat4(1.f), glm::vec3(20.f, 20.f, 0.f)));
		//
		//RenderCommand::StencilDraw(2);
		//
		//Renderer::Submit(m_Shader, m_Door2, m_Door2transform);
		//
		//RenderCommand::StencilDraw(3);

		m_Shader->Bind();
		m_Shader->SetUniform3f("lightpos", m_Lightpos);

		Renderer::Submit(m_Shader, m_Skybox);
		Renderer::Submit(m_Shader, m_Box1, glm::translate(glm::mat4(1.f), glm::vec3(20.f, 70.f, 0.f)));
		Renderer::Submit(m_Shader, m_Box2, glm::translate(glm::mat4(1.f), glm::vec3(70.f, 70.f, 0.f)));
		Renderer::Submit(m_Shader, m_Box3, glm::translate(glm::mat4(1.f), glm::vec3(20.f, 20.f, 0.f)));

		glm::mat4 t = m_Door1transform * glm::inverse(m_Door2transform * glm::rotate(glm::mat4(1.f), angle, glm::vec3(0.f, 0.f, 1.f)));;

		Renderer::Submit(m_Shader, m_Camerablock, trans2 * glm::inverse(m_Camera->GetViewMatrix()));

		RenderCommand::EnabledWireframe(true);
		Renderer::Submit(m_Shader, m_Door2, m_Door2transform * glm::rotate(glm::mat4(1.f), glm::pi<float>() + angle, glm::vec3(0.f, 0.f, 1.f)));
		Renderer::Submit(m_Shader, m_Door2, m_Door2transform * glm::rotate(glm::mat4(1.f), angle, glm::vec3(0.f, 0.f, 1.f)));
		//Renderer::Submit(m_Shader, m_Door1, m_Door1transform);
		Renderer::Submit(m_Shader, m_Door1, m_Door1transform * glm::rotate(glm::mat4(1.f), glm::pi<float>(), glm::vec3(0.f, 0.f, 1.f)));
		RenderCommand::EnabledWireframe(false);

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
			ImGui::SliderFloat("Angle: ", &angle, 0.f, glm::two_pi<float>());
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
