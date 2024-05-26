
#include <LSEngine.h>

#include "LSEngine/ECS/Entity.h"
#include "LSEngine/ECS/Objects/ECS.h"

#include "layers/Engine.h"
#include "layers/Editor.h"

#include "LSEngine/Core/EntryPoint.h"

class Sandbox : public LSE::Application
{
public:
	Sandbox()
		: Application(1920, 1080)
	{
		PushLayer(MakeScope<EngineLayer>());
		PushLayer(MakeScope<EditorLayer>());
	}

	~Sandbox()
	{

	}
};

LSE::Application* LSE::CreateApplication()
{
	return new Sandbox();
}
