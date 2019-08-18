#pragma once

#include <LSEngine/Events/Event.h>

namespace LSE {

	class LSE_API WindowEvent : public Event
	{
	public:
		EVENT_CLASS_CATEGORY(EventCategoryWindow);
	};

	class LSE_API WindowCloseEvent : public WindowEvent
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose);
	};

	class LSE_API WindowResizeEvent : public WindowEvent
	{
	public:
		WindowResizeEvent(int width, int height)
			: m_Width(width), m_Height(height)
		{
		
		}

		inline const int& GetWidth() const { return m_Width; }
		inline const int& GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize);
	private:
		int m_Width, m_Height;
	};

	class LSE_API WindowFocusEvent : public WindowEvent
	{
	public:
		WindowFocusEvent() {}

		EVENT_CLASS_TYPE(WindowFocus);
	};

	class LSE_API WindowLostFocusEvent : public WindowEvent
	{
	public:
		WindowLostFocusEvent() {}

		EVENT_CLASS_TYPE(WindowLostFocus);
	};

	class LSE_API WindowMovedEvent : public WindowEvent
	{
	public:
		WindowMovedEvent(int x, int y)
			: m_WindowX(x), m_WindowY(y)
		{
		
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowMovedEvent: " << m_WindowX << ", " << m_WindowY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowMoved);
	private:
		int m_WindowX, m_WindowY;
	};

	class LSE_API AppEvent : public Event
	{
	public:
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class LSE_API AppTickEvent : public AppEvent
	{
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick);
	};

	class LSE_API AppUpdateEvent : public AppEvent
	{
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate);
	};

	class LSE_API AppRenderEvent : public AppEvent
	{
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender);
	};

}