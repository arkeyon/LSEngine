#include "lsepch.h"

#include "LSEngine/Core/Application.h"
#include "LSEngine/Core/Input.h"
#include "LSEngine/Core/Timer.h"
#include "LSEngine/Core/IOUtils.h"
#include "LSEngine/Core/Camera/PerspectiveCamera.h"
#include "LSEngine/Core/Camera/OrthographicCamera.h"

#include "LSEngine/Renderer/Shader.h"
#include "LSEngine/Renderer/Buffer.h"
#include "LSEngine/Renderer/VertexArray.h"
#include "LSEngine/Renderer/Renderer.h"
#include "LSEngine/Renderer/Meshfactory.h"

#include "Platform/Windows/WindowsTimer.h"

#include "FreeImage.h"
#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace LSE {

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Scope<Application> Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = Scope<Application>(this);

		FreeImage_Initialise();

		m_Window = Scope<Window>(Window::Create(WindowProps("Title", 1280, 720)));
		m_Window->SetVSync(false);
		m_Window->SetEventCallbackFn(BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = MakeRef<ImGuiLayer>();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{

	}

	void Application::PushLayer(Ref<Layer> layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Ref<Layer> overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
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

		Timer& timer = WindowsTimer();

		while (m_Running)
		{
			float delta = timer.elapsed();
			timer.reset();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(delta);
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}
}