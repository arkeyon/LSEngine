#include <LSEngine.h>

#include "imgui.h"

class ExampleLayer : public LSE::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
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
		ImGui::Begin("Test");
		ImGui::Text("Hello World!");
		ImGui::End();
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
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

LSE::Application* LSE::CreateApplication()
{
	return new Sandbox();
}
