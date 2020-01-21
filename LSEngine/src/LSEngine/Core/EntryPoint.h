#pragma once

#include "Core.h"

#ifdef LSE_PLATFORM_WINDOWS

extern LSE::Application* LSE::CreateApplication();

int main(int argc, char** argv)
{
	LSE::Log::Init();
	LSE_CORE_WARN("Initialized Log!");

	auto app = LSE::CreateApplication();
	app->Run();
	delete app;
}

#endif