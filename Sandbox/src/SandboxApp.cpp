#include <LSEngine.h>

#include "imgui.h"

#include "LSEngine/Core/Core.h"
#include "LSEngine/Core/IOUtils.h"
#include "LSEngine/Renderer/VertexArray.h"
#include "LSEngine/Renderer/Meshfactory.h"
#include "LSEngine/Renderer/Renderer.h"
#include "LSEngine/Core/Camera/PerspectiveCamera.h"
#include "LSEngine/Core/Camera/OrthographicCamera.h"
#include "LSEngine/Renderer/Shader.h"
#include "LSEngine/Renderer/Texture.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/color_space.hpp>

class ExampleLayer : public LSE::Layer
{
private:
	LSE::Ref<LSE::VertexArray> m_VertexArray;
	LSE::Ref<LSE::Shader> m_Shader;
	LSE::Ref<LSE::Camera3D> m_Camera;

	bool m_Paused = false;

	LSE::Ref<LSE::Texture2D> m_TestTexture;

	float m_MoveSpeed = 20.f;
	float m_RotateSpeed = 8.f;

	float m_FPS = 0.f;
	float m_Frames = 0.f;
	float m_Time = 0.f;

	glm::vec2 m_PositionTickSum;

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

		m_Shader.reset(Shader::Create("assets/shaders/simpleshader.vert", "assets/shaders/simpleshader.frag"));
		m_VertexArray.reset(VertexArray::Create());
		m_Camera.reset(new PerspCamera3D(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::two_pi<float>() / 6.f, 16.f / 9.f, 0.1f, 10000.f));

		{

			const int width = 22;
			const int height = 4;

			vertex_t* vertices = new vertex_t[(width + 1) * (height + 1)];
			uint32_t* indices = new uint32_t[width * height * 6];
			vertex_t temp;
			temp.a_Colour = glm::vec4(1.f, 1.f, 1.f, 1.f);
			temp.a_Tex = 0;
			MeshFactory::generateSphere(vertices, (uint32_t*)indices, 2.f, width, height, temp);

			Ref<VertexBuffer> vertexBuffer(VertexBuffer::Create((width + 1)* (height + 1) * sizeof(vertex_t), vertices));
			vertexBuffer->SetLayout({
				{ SDT::Float3, "a_Position" },
				{ SDT::Float4, "a_Colour" },
				{ SDT::Float3, "a_Normal" },
				{ SDT::Float2, "a_UV" },
				{ SDT::Float, "a_Tex" }
			});

			Ref<IndexBuffer> indexBuffer(IndexBuffer::Create(width* height * 6, indices));

			m_VertexArray->AddVertexBuffer(vertexBuffer);
			m_VertexArray->SetIndexBuffer(indexBuffer);
		}

		RenderCommand::SetClearColour(glm::vec4(0.f, 0.f, 0.f, 1.f));
		RenderCommand::EnableFaceCulling(true);
		RenderCommand::EnableDepthTest(true);

		m_TestTexture = Texture2D::Create("assets/textures/TEST.png");

		auto& window = Application::Get().GetWindow();
		window.SetCursorState(false);
	}

	void OnUpdate(float delta) override
	{
		using namespace LSE;

		m_Frames += 1.f;
		m_Time += delta;

		if (m_Time > 1.f)
		{
			m_FPS = m_Frames / m_Time;
			m_Frames = 0.f;
			m_Time = 0.f;
		}

		RenderCommand::Clear();

		if (!m_Paused)
		{
			m_Camera->MoveLocalView(
				glm::vec3(
					Input::IsKeyPressed(LSE_KEY_D) - Input::IsKeyPressed(LSE_KEY_A),
					Input::IsKeyPressed(LSE_KEY_SPACE) - Input::IsKeyPressed(LSE_KEY_LEFT_SHIFT),
					Input::IsKeyPressed(LSE_KEY_W) - Input::IsKeyPressed(LSE_KEY_S)
				) * m_MoveSpeed * delta,
				glm::vec3(
					m_PositionTickSum.y,
					m_PositionTickSum.x,
					0.f
				) * m_RotateSpeed * delta
			);
		}

		m_PositionTickSum = glm::vec2();

		m_Shader->SetUniform4f("u_Color", m_Color);
		m_Shader->SetUniform3f("u_AmbientColor", m_AmbientColor);
		m_Shader->SetUniform3f("u_DiffuseColor", m_DiffuseColor);
		m_Shader->SetUniform3f("u_SpecularColor", m_SpecularColor);
		m_Shader->SetUniform1f("u_Shininess", m_Shininess);

		m_Shader->SetUniformi("tex", 0);
		m_TestTexture->Bind(0);

		Renderer::BeginScene(m_Camera);
		Renderer::Submit(m_Shader, m_VertexArray);
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
		if (e.GetEventType() == EventType::MouseMoved)
		{
			auto& me = (MouseMovedEvent&)e;

			m_PositionTickSum -= glm::vec2(me.GetDX(), me.GetDY());
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
