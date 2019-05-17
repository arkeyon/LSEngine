#pragma once

#include "Core.h"

namespace LSE {

	class LSE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	Application* CreateApplication();

}