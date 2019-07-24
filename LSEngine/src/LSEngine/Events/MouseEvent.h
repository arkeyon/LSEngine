#pragma once

#include "Event.h"

namespace LSE {

	class LSE_API MouseEvent : public Event
	{
	public:
		EVENT_CLASS_CATEGORY(EventCategoryMouse);
	};

	class LSE_API MouseMovedEvent : public MouseEvent
	{
	public:
		MouseMovedEvent(float x, float y)
			: m_MouseX(x), m_MouseY(y)
		{

		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved);

	private:
		float m_MouseX, m_MouseY;
	};

	class LSE_API MouseScrolledEvent : public MouseEvent
	{
	public:
		MouseScrolledEvent(float scroll)
			: m_Scroll(scroll)
		{

		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_Scroll;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled);

	private:
		float m_Scroll;
	};

	class LSE_API MouseButtonEvent : public MouseEvent
	{
	public:
		MouseButtonEvent(int button)
			: m_Button(button)
		{

		}

	protected:
		int m_Button;
	};

	class LSE_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button)
		{

		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed);
	};

	class LSE_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button)
		{

		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased);
	};
}