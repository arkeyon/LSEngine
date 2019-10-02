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

	}

	void OnImGuiRender() override
	{

	}

	void OnEvent(LSE::Event& e) override
	{

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
