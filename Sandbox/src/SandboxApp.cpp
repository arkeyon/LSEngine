#include <LSEngine.h>

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