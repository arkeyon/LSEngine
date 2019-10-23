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

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running = true;
		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();

}