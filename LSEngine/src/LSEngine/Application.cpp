#include "lsepch.h"
#include "Application.h"
#include "Input.h"

#include <glad/glad.h>

#include "Timer.h"

#include <glm/glm.hpp>

namespace LSE {

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::m_Instance = nullptr;

	Application::Application()
	{
		m_Instance = this;

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

#define KEYDOWN(x) (GetAsyncKeyState(x) & 0x8000)

	void Application::Run()
	{
		while (m_Running)
		{
			glClear(GL_COLOR_BUFFER_BIT);

			if (KEYDOWN(VK_NUMPAD9)) continue;

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