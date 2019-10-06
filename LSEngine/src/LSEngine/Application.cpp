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
#include "Renderer/Camera.h"

#include "FreeImage.h"
#include "imgui.h"

#include <glm/glm.hpp>
#include <memory>

namespace LSE {

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	int tex = 0;

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
		
		struct vertex_t
		{
			glm::vec3 a_Position;
			glm::vec4 a_Colour;
			glm::vec2 a_UV;
			float a_Tex;
		};

		vertex_t vertices[8];
		uint32_t indices[36];
		generateRectCenter((float*)vertices, (int32_t*)indices, sizeof(vertex_t), 0, 5.f, 5.f, 5.f);
		for (int i = 0; i < 8; i++)
		{
			vertices[i].a_Colour = glm::vec4(0.5f, 0.5f, 0.5f, 1.f);
			vertices[i].a_UV = glm::vec2(0.f, 0.f);
			vertices[i].a_Tex = -1.f;
		}

		std::shared_ptr<VertexBuffer> vertexBuffer(VertexBuffer::Create(4 * 9, (float*)vertices));
		vertexBuffer->SetLayout({
			{ SDT::Float3, "a_Position" },
			{ SDT::Float4, "a_Colour" },
			{ SDT::Float2, "a_UV" },
			{ SDT::Float, "a_Tex" }
			});
		
		std::shared_ptr<IndexBuffer> indexBuffer(IndexBuffer::Create(6, indices));
		
		tex = LSE::CreateTexture("Gates/XNOR.png");

		shader.SetUniformi("tex", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);

		vertexArray->AddVertexBuffer(vertexBuffer);
		vertexArray->SetIndexBuffer(indexBuffer);

		RenderCommand::SetClearColour(glm::vec4(0.6f, 0.6f, 0.6f, 1.f));

		Camera3D camera(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f));

		while (m_Running)
		{
			RenderCommand::Clear();

			shader.Bind();
			Renderer::BeginScene();
			Renderer::Submit(vertexArray);
			Renderer::EndScene();
			shader.Unbind();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();
			m_ImGuiLayer->Begin();
			ImGui::Begin("Contents Window", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground);
			if (ImGui::ImageButton((void*)tex, ImVec2(64, 64), ImVec2(0, 0), ImVec2(1, 1), 1, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1)))
			{

			}
			ImGui::End();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}
}