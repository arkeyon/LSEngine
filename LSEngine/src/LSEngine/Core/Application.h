#pragma once

#include "Core.h"

#include "LSEngine/Events/Event.h"
#include "LSEngine/Events/ApplicationEvent.h"
#include "LSEngine/Core/Window.h"
#include "LSEngine/Core/LayerStack.h"
#include "LSEngine/ImGui/ImGuiLayer.h"

namespace LSE {

	class LSE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Ref<Layer> layer);
		void PushOverlay(Ref<Layer> overlay);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }
	private:
		Scope<Window> m_Window;
		Ref<ImGuiLayer> m_ImGuiLayer;

		bool m_Running = true;
		LayerStack m_LayerStack;
	private:
		static Scope<Application> s_Instance;
	};

	Application* CreateApplication();

}