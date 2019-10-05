#include <LSEngine.h>

#include "imgui.h"

#include "LSEngine/IOUtils.h"

class LogicLayer : public LSE::Layer
{
public:
	LogicLayer()
		: Layer("LogicLayer")
	{

	}

	void OnUpdate() override
	{
		if (LSE::Input::IsKeyPressed(LSE_KEY_TAB))
		{
			LSE_TRACE("Tab is pressed (pol)!");
		}
	}

	void OnImGuiRender() override
	{
	//	ImGui::ImageButton((void*)tex, ImVec2(), ImVec2(0.f, 0.f), ImVec2(1.f, 1.f), -1, ImVec4(0.f, 0.f, 0.f, 0.f), ImVec4(1.f, 1.f, 1.f, 1.f));
	}

	void OnEvent(LSE::Event& e) override
	{
		if (e.GetEventType() == LSE::EventType::KeyPressed)
		{
			LSE::KeyPressedEvent& ke = (LSE::KeyPressedEvent&)e;
			if (ke.GetKeyCode() == LSE_KEY_TAB)
			{
				LSE_TRACE("Tab is pressed (event)!");
			}
		}
	}
};

class Sandbox : public LSE::Application
{
public:
	Sandbox()
	{
		PushLayer(new LogicLayer());
	}

	~Sandbox()
	{

	}
};

LSE::Application* LSE::CreateApplication()
{
	return new Sandbox();
}
