#pragma once

#include "LSEngine/Core/Window.h"

#include <GLFW/glfw3.h>

namespace LSE {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);

		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		inline void SetEventCallbackFn(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; };
		void SetVSync(bool enabled) override;
		inline bool IsVSync() override { return m_Data.VSync; }
		virtual void SetCursorState(bool enabled) override;

		virtual void* GetNativeWindow() override { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool CursorState;
			bool VSync;
			EventCallbackFn EventCallback;
		} m_Data;
	};
}