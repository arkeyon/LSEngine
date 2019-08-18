#pragma once

#include "Event.h"

namespace LSE {

	class LSE_API KeyEvent : public Event
	{
	public:
		inline const int& GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard);
	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode)
		{

		}

	protected:
		int m_KeyCode;
	};

	class LSE_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode)
			: KeyEvent(keycode)
		{

		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed);
	};

	class LSE_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode)
		{

		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased);
	};

	class LSE_API KeyRepeatedEvent : public KeyEvent
	{
	public:
		KeyRepeatedEvent(int keycode)
			: KeyEvent(keycode)
		{

		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyRepeatedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyRepeated);
	};

	class LSE_API KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode)
		{

		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped);
	};
}