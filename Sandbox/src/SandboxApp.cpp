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
#include <glm/gtx/color_space.hpp>

class ExampleLayer : public LSE::Layer
{
private:
	std::shared_ptr <LSE::VertexArray> m_VertexArray;
	std::shared_ptr<LSE::Shader> m_Shader;
	std::shared_ptr<LSE::Camera3D> m_Camera;
	std::shared_ptr<LSE::PerspCamera3D> m_PerspCamera;
	std::shared_ptr<LSE::OrthoCamera3D> m_OrthoCamera;

	float m_MoveSpeed = 20.f;
	float m_RotateSpeed = 2.f;
	bool m_Depthtest = true;
	bool m_Facecull = true;
	bool m_Projection = true;
	glm::vec4 m_Colour = glm::vec4(1.f, 1.f, 1.f, 1.f);

	float m_FPS = 0.f;
	float m_Frames = 0.f;
	float m_Time = 0.f;
public:
	ExampleLayer()
		: Layer("ExampleLayer")
	{
		using namespace LSE;

		RendererAPI::SetAPI(RendererAPI::API::OpenGL);

		m_Shader.reset(Shader::Create("simpleshader.vert", "simpleshader.frag"));
		m_VertexArray.reset(VertexArray::Create());
		m_PerspCamera.reset(new PerspCamera3D(glm::vec3(0.f, 0.f, 0.f), glm::vec3(-glm::two_pi<float>() / 10.f, 0.f, 0.f), glm::two_pi<float>() / 6.f, 16.f / 9.f, 0.1f, 10000.f));
		m_OrthoCamera.reset(new OrthoCamera3D(glm::vec3(0.f, 0.f, 0.f), glm::vec3(-glm::two_pi<float>() / 10.f, 0.f, 0.f), glm::two_pi<float>() / 6.f, -100.f, 100.f, -50.f, 50.f, -5000.f, 5000.f));
		m_Camera = m_PerspCamera;
		{
			vertex_t vertices[8];
			uint32_t indices[36];
			MeshFactory::generateRectCenter(vertices, (uint32_t*)indices, 5.f, 5.f, 5.f);
			vertices[0].a_Colour = glm::vec4(glm::rgbColor(glm::vec3(45.f, 1.f, 1.f)), 1.f);
			vertices[1].a_Colour = glm::vec4(glm::rgbColor(glm::vec3(45.f, 1.f, 1.f)), 1.f);
			vertices[2].a_Colour = glm::vec4(glm::rgbColor(glm::vec3(45.f, 1.f, 1.f)), 1.f);
			vertices[3].a_Colour = glm::vec4(glm::rgbColor(glm::vec3(45.f, 1.f, 1.f)), 1.f);
			vertices[4].a_Colour = glm::vec4(glm::rgbColor(glm::vec3(180.f, 1.f, 1.f)), 1.f);
			vertices[5].a_Colour = glm::vec4(glm::rgbColor(glm::vec3(180.f, 1.f, 1.f)), 1.f);
			vertices[6].a_Colour = glm::vec4(glm::rgbColor(glm::vec3(180.f, 1.f, 1.f)), 1.f);
			vertices[7].a_Colour = glm::vec4(glm::rgbColor(glm::vec3(180.f, 1.f, 1.f)), 1.f);

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
		//RenderCommand::EnableDepthTest();
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

		m_Camera->MoveLocalView(
			glm::vec3(
				Input::IsKeyPressed(LSE_KEY_D) - Input::IsKeyPressed(LSE_KEY_A),
				Input::IsKeyPressed(LSE_KEY_SPACE) - Input::IsKeyPressed(LSE_KEY_LEFT_SHIFT),
				Input::IsKeyPressed(LSE_KEY_W) - Input::IsKeyPressed(LSE_KEY_S)
			) * m_MoveSpeed * delta,
			glm::vec3(
				Input::IsKeyPressed(LSE_KEY_UP) - Input::IsKeyPressed(LSE_KEY_DOWN),
				Input::IsKeyPressed(LSE_KEY_LEFT) - Input::IsKeyPressed(LSE_KEY_RIGHT),
				0.f
			) * m_RotateSpeed * delta
		);

		m_Shader->Bind();
		Renderer::BeginScene(*m_Camera);
		m_Shader->SetUniformMat4("u_VP", m_Camera->GetVP());

		RenderCommand::EnableFaceCulling(m_Facecull);
		RenderCommand::EnableDepthTest(m_Depthtest);
		RenderCommand::EnabledWireframe(false);

		if (m_Projection)
		{
			m_PerspCamera->SetView(m_Camera->GetPos(), m_Camera->GetAngles());
			m_Camera = m_PerspCamera;
		}
		else
		{
			m_OrthoCamera->SetView(m_Camera->GetPos(), m_Camera->GetAngles());
			m_Camera = m_OrthoCamera;
		}

		m_Shader->SetUniform4fv("u_Colour", m_Colour);
		for (int y = 0; y < 100; y++)
		{
			for (int x = 0; x < 100; x++)
			{
				m_Shader->SetUniformMat4("u_ModelMatrix", glm::translate(glm::mat4(1.f), glm::vec3(x * 5.5f, y * 5.5f, 0.f)));
				Renderer::Submit(m_VertexArray);
			}
		}
		
		Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Debug");
		ImGui::Text((std::string("FPS: ") + std::to_string(m_FPS)).c_str());
		ImGui::ColorEdit4("Colour", &m_Colour[0]);
		ImGui::Checkbox("Depth test", &m_Depthtest);
		ImGui::Checkbox("Cull face", &m_Facecull);
		ImGui::Checkbox("Projection", &m_Projection);
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
