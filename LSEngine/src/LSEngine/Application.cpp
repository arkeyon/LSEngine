#include "Application.h"

#include "Events/ApplicationEvent.h"

namespace LSE {

	Application::Application()
	{

	}


	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategory::EventCategoryWindow))
		{
			LSE_TRACE(e);
		}

		while (true);
	}
}