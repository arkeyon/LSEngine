#include "lsepch.h"
#include "Application.h"
#include "Input.h"

#include <glad/glad.h>

#include "Timer.h"

#include <glm/glm.hpp>
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Renderer.h"

#include <memory>

namespace LSE {

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;

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

		{

			float vertices[] =
			{
				-1.f, -1.f,		+1.f, +0.f, +0.f, +1.f,
				-0.5f, +1.f,	+0.f, +1.f, +0.f, +1.f,
				+0.f,-1.f,		+0.f, +0.f, +1.f, +1.f
			};

			uint32_t indices[] =
			{
				0, 1, 2
			};

			std::shared_ptr<VertexBuffer> vertexBuffer(VertexBuffer::Create(3 * 6, vertices));
			vertexBuffer->SetLayout({
				{ SDT::Float2, "a_Position" },
				{ SDT::Float4, "a_Colour" }
				});

			std::shared_ptr<IndexBuffer> indexBuffer(IndexBuffer::Create(3, indices));

			vertexArray->AddVertexBuffer(vertexBuffer);
			vertexArray->SetIndexBuffer(indexBuffer);
		}

		std::shared_ptr <VertexArray> vertexArray2(VertexArray::Create());
		
		{
		
			float vertices[] =
			{
				+0.f, -1.f,		+1.f, +0.f, +0.f, +1.f,
				+0.f, +1.f,		+0.f, +1.f, +0.f, +1.f,
				+1.f, +1.f,		+0.f, +0.f, +1.f, +1.f,
				+1.f, -1.f,		+0.f, +1.f, +1.f, +1.f
			};
		
			uint32_t indices[] =
			{
				0, 1, 2, 2, 3, 0
			};
		
			std::shared_ptr<VertexBuffer> vertexBuffer(VertexBuffer::Create(4 * 6, vertices));
			vertexBuffer->SetLayout({
				{ SDT::Float2, "a_Position" },
				{ SDT::Float4, "a_Colour" }
				});
		
			std::shared_ptr<IndexBuffer> indexBuffer(IndexBuffer::Create(6, indices));
		
			vertexArray2->AddVertexBuffer(vertexBuffer);
			vertexArray2->SetIndexBuffer(indexBuffer);
		}

		while (m_Running)
		{
			RenderCommand::Clear();

			shader.Bind();
			Renderer::BeginScene();
			Renderer::Submit(vertexArray);
			Renderer::Submit(vertexArray2);
			Renderer::EndScene();
			shader.Unbind();

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