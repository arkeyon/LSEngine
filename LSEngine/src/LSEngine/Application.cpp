#include "lsepch.h"

#include "Application.h"
#include "Input.h"
#include "Timer.h"
#include "IOUtils.h"

#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Renderer.h"
#include "Renderer/Meshfactory.h"
#include "Renderer/Camera/PerspectiveCamera.h"

#include "FreeImage.h"
#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

namespace LSE {

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;

		FreeImage_Initialise();

		m_Window = std::unique_ptr<Window>(Window::Create(WindowProps("Title", 1280, 720)));
		m_Window->SetEventCallbackFn(BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		LSE_CORE_TRACE("{0}", e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>([&](WindowCloseEvent& e)
			{
				m_Running = false;
				return true;
			});
	
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.m_Handled)
				break;
		}
	}

	void Application::Run()
	{
		RendererAPI::SetAPI(RendererAPI::API::OpenGL);
		Shader shader("simpleshader.vert", "simpleshader.frag");
		
		std::shared_ptr <VertexArray> vertexArray(VertexArray::Create());

		vertex_t vertices[8];
		uint32_t indices[36];
		generateRectCenter(vertices, (uint32_t*)indices, 5.f, 5.f, 5.f);
		for (int i = 0; i < 8; i++) vertices[i].a_Colour = glm::vec4(0.5f, 0.5f, 0.5f, 1.f);

		std::shared_ptr<VertexBuffer> vertexBuffer(VertexBuffer::Create(sizeof(vertex_t) / 4 * 8, (float*)vertices));
		vertexBuffer->SetLayout({
			{ SDT::Float3, "a_Position" },
			{ SDT::Float4, "a_Colour" },
			{ SDT::Float2, "a_UV" },
			{ SDT::Float, "a_Tex" }
			});
		
		std::shared_ptr<IndexBuffer> indexBuffer(IndexBuffer::Create(36, indices));

		vertexArray->AddVertexBuffer(vertexBuffer);
		vertexArray->SetIndexBuffer(indexBuffer);

		RenderCommand::SetClearColour(glm::vec4(0.f, 0.f, 0.f, 1.f));

		Camera3D& camera = PerspCamera3D(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::two_pi<float>() / 6.f, 16.f / 9.f, 0.1f, 100.f);

		float movespeed = 0.1f;
		float rotatespeed = 0.01f;

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		while (m_Running)
		{
			RenderCommand::Clear();

			camera.MoveLocalView(
				glm::vec3(
					Input::IsKeyPressed(LSE_KEY_D) - Input::IsKeyPressed(LSE_KEY_A),
					Input::IsKeyPressed(LSE_KEY_SPACE) - Input::IsKeyPressed(LSE_KEY_LEFT_SHIFT),
					Input::IsKeyPressed(LSE_KEY_W) - Input::IsKeyPressed(LSE_KEY_S)
				) * movespeed,
				glm::vec3(
					Input::IsKeyPressed(LSE_KEY_UP) - Input::IsKeyPressed(LSE_KEY_DOWN), 
					Input::IsKeyPressed(LSE_KEY_LEFT) - Input::IsKeyPressed(LSE_KEY_RIGHT),
					0.f
				) * rotatespeed
			);

			shader.Bind();
			shader.SetUniformMat4("u_VP", camera.GetVP());
			Renderer::BeginScene();
			Renderer::Submit(vertexArray);
			Renderer::EndScene();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}
}