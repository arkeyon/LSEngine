#include <LSEngine.h>

#include "imgui.h"

#include "LSEngine/IOUtils.h"
#include "LSEngine/Renderer/VertexArray.h"
#include "LSEngine/Renderer/Meshfactory.h"
#include "LSEngine/Renderer/Renderer.h"
#include "LSEngine/Renderer/Camera/PerspectiveCamera.h"
#include "LSEngine/Renderer/Camera/OrthographicCamera.h"
#include "LSEngine/Renderer/Shader.h"

#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public LSE::Layer
{
private:
	std::shared_ptr <LSE::VertexArray> m_VertexArray;
	std::shared_ptr<LSE::Shader> m_Shader;
	std::shared_ptr<LSE::Camera3D> m_Camera;
	std::shared_ptr<LSE::PerspCamera3D> m_PerspCamera;
	std::shared_ptr<LSE::OrthoCamera3D> m_OrthoCamera;
	glm::vec3 m_Colour;
	bool m_CameraSelect = true;
	bool m_LastSelect = true;

	float m_MoveSpeed = 0.1f;
	float m_RotateSpeed = 0.02f;
public:
	ExampleLayer()
		: Layer("ExampleLayer"), m_Colour(glm::vec3(1.f, 1.f, 1.f))
	{
		using namespace LSE;

		RendererAPI::SetAPI(RendererAPI::API::OpenGL);

		m_Shader.reset(Shader::Create("simpleshader.vert", "simpleshader.frag"));
		m_VertexArray.reset(VertexArray::Create());
		m_PerspCamera.reset(new PerspCamera3D(glm::vec3(-10.f, 0.f, 5.f), glm::vec3(-glm::two_pi<float>() / 10.f, 0.f, 0.f), glm::two_pi<float>() / 6.f, 16.f / 9.f, 0.1f, 100.f));
		m_OrthoCamera.reset(new OrthoCamera3D(glm::vec3(-10.f, 0.f, 5.f), glm::vec3(-glm::two_pi<float>() / 10.f, 0.f, 0.f), 16.f / 9.f, -10.f, 10.f, -10.f, 10.f, 0.1f, 100.f));
		m_Camera = m_PerspCamera;

		{
			vertex_t vertices[8];
			uint32_t indices[36];
			MeshFactory::generateRectCenter(vertices, (uint32_t*)indices, 5.f, 5.f, 5.f);
			for (int i = 0; i < 8; i++) vertices[i].a_Colour = glm::vec4(0.5f, 0.5f, 0.5f, 1.f);

			std::shared_ptr<VertexBuffer> vertexBuffer(VertexBuffer::Create(sizeof(vertex_t) / 4 * 8, (float*)vertices));
			vertexBuffer->SetLayout({
				{ SDT::Float3, "a_Position" },
				{ SDT::Float4, "a_Colour" },
				{ SDT::Float2, "a_UV" },
				{ SDT::Float, "a_Tex" }
				});

			std::shared_ptr<IndexBuffer> indexBuffer(IndexBuffer::Create(36, indices));

			m_VertexArray->AddVertexBuffer(vertexBuffer);
			m_VertexArray->SetIndexBuffer(indexBuffer);
		}

		RenderCommand::SetClearColour(glm::vec4(0.f, 0.f, 0.f, 1.f));
		RenderCommand::EnableDepthTest();
		RenderCommand::EnableFaceCulling();
	}

	void OnUpdate() override
	{
		using namespace LSE;

		RenderCommand::Clear();

		if (m_CameraSelect != m_LastSelect)
		{
			if (m_CameraSelect)
			{
				m_Camera = m_PerspCamera;
				m_PerspCamera->SetView(m_OrthoCamera->GetPos(), m_OrthoCamera->GetAngles());
			}
			else
			{
				m_Camera = m_OrthoCamera;
				m_OrthoCamera->SetView(m_PerspCamera->GetPos(), m_PerspCamera->GetAngles());
			}
			m_LastSelect = m_CameraSelect;
		}
		m_Camera->MoveLocalView(
			glm::vec3(
				Input::IsKeyPressed(LSE_KEY_D) - Input::IsKeyPressed(LSE_KEY_A),
				Input::IsKeyPressed(LSE_KEY_SPACE) - Input::IsKeyPressed(LSE_KEY_LEFT_SHIFT),
				Input::IsKeyPressed(LSE_KEY_W) - Input::IsKeyPressed(LSE_KEY_S)
			) * m_MoveSpeed,
			glm::vec3(
				Input::IsKeyPressed(LSE_KEY_UP) - Input::IsKeyPressed(LSE_KEY_DOWN),
				Input::IsKeyPressed(LSE_KEY_LEFT) - Input::IsKeyPressed(LSE_KEY_RIGHT),
				0.f
			) * m_RotateSpeed
		);

		m_Shader->Bind();
		m_Shader->SetUniform4fv("u_Colour", glm::vec4(m_Colour, 1.f));
		m_Shader->SetUniformMat4("u_VP", m_Camera->GetVP());
		Renderer::BeginScene();
		Renderer::Submit(m_VertexArray);
		Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::GetIO().MouseDrawCursor = true;
		ImGui::Begin("Settings");
		ImGui::Checkbox("Camera", &m_CameraSelect);
		ImGui::End();
	}

	void OnEvent(LSE::Event& e) override
	{

	}
};

class Sandbox : public LSE::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

LSE::Application* LSE::CreateApplication()
{
	return new Sandbox();
}
