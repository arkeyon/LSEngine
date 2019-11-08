#pragma once

#include "Core.h"

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace LSE {

	class LSE_API Log
	{
	public:
		static void Init();

		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}

#define LSE_CORE_TRACE(...) ::LSE::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LSE_CORE_INFO(...)  ::LSE::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LSE_CORE_WARN(...)  ::LSE::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LSE_CORE_CRITICAL(...) ::LSE::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define LSE_CORE_ERROR(...) ::LSE::Log::GetCoreLogger()->error(__VA_ARGS__)

#define LSE_TRACE(...) ::LSE::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LSE_INFO(...)  ::LSE::Log::GetClientLogger()->info(__VA_ARGS__)
#define LSE_WARN(...)  ::LSE::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LSE_CRITICAL(...) ::LSE::Log::GetClientLogger()->critical(__VA_ARGS__)
#define LSE_ERROR(...) ::LSE::Log::GetClientLogger()->error(__VA_ARGS__)