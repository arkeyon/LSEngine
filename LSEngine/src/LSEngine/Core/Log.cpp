#include "lsepch.h"
#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace LSE {

	Ref<spdlog::logger> Log::s_CoreLogger;
	Ref<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_CoreLogger = spdlog::stderr_color_mt("LSENGINE");
		s_CoreLogger->set_level(spdlog::level::trace);
		s_ClientLogger = spdlog::stderr_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}