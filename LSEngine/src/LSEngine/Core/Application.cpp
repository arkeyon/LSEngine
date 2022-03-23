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

#include "LSEngine/Events/MouseEvent.h"

#include "Platform/Windows/WindowsTimer.h"

#include "FreeImage.h"
#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "LSEngine/ECS/ECSManager.h"

namespace LSE {

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(unsigned int width, unsigned int height, bool fullscreen)
	{
		s_Instance = this;

		FreeImage_Initialise();

		m_Window = Scope<Window>(Window::Create(WindowProps("Title", width, height, fullscreen)));
		m_Window->SetVSync(false);
		m_Window->SetEventCallbackFn(BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = MakeRef<ImGuiLayer>();

		ECS->Init();

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
		dispatcher.Dispatch<WindowCloseEvent>([&](WindowCloseEvent& e) { return OnWindowClose(e); });
		dispatcher.Dispatch<WindowResizeEvent>([&](WindowResizeEvent& e) { return OnWindowResize(e); });

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			auto layer = *--it;

			if (layer->isActive())
			{
				layer->OnEvent(e);
				if (e.m_Handled)
					break;
			}
		}
	}

	void Application::Run()
	{

		Timer& timer = WindowsTimer();

		while (m_Running)
		{
			float delta = timer.elapsed();
			if (delta < 1.f / 144.f) continue;
			timer.reset();
		
			if (!m_Minimized)
			{
				for (Ref<Layer>& layer : m_LayerStack)
					layer->OnUpdate(delta);
			}
		
			m_ImGuiLayer->Begin();
			for (Ref<Layer>& layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();
		
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		LSE_CORE_TRACE("Window Close");
		m_Running = false;
		ECS->Destroy();
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}