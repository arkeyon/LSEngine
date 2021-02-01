#pragma once

#include "LSEngine/Core/Core.h"
#include "LSEngine/Events/Event.h"

#include "LSEngine/Renderer/GraphicsContext.h"

namespace LSE {

	struct WindowProps
	{
		std::string Title;
		unsigned int Width, Height;
		bool Fullscreen;

		WindowProps()
			:Title("DefaultTitle"), Width(640), Height(480)
		{

		}

		WindowProps(std::string title, unsigned int width, unsigned int height, bool fullscreen)
			:Title(title), Width(width), Height(height), Fullscreen(fullscreen)
		{

		}
	};

	class LSE_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallbackFn(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() = 0;
		virtual void SetCursorState(bool visible) = 0;

		virtual void* GetNativeWindow() = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	protected:
		static GraphicsContext* s_Context;
	};

}