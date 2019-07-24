#include <LSEngine.h>

#include <LSEngine/Events/KeyEvent.h>

class Sandbox : public LSE::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

LSE::Application* LSE::CreateApplication()
{
	return new Sandbox();
}
