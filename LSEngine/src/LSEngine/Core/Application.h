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
		Application(unsigned int width, unsigned int height, bool fullscreen = false);
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
		bool m_Minimized = false;
		LayerStack m_LayerStack;
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		static Application* s_Instance;
	};

	Application* CreateApplication();

}