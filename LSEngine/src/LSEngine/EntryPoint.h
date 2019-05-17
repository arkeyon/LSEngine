#pragma once

#include "Core.h"

#ifdef LSE_PLATFORM_WINDOWS

extern LSE::Application* LSE::CreateApplication();

int main(int argc, char** argv)
{
	auto app = LSE::CreateApplication();
	app->Run();
	delete app;
}

#endif