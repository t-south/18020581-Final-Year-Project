#pragma once
//#include "Core.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace geProject {

	class Logging
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetEngineLog() { return coreLog; }
		inline static std::shared_ptr<spdlog::logger>& GetGameLog() { return gameLog; }
	private:
		static std::shared_ptr<spdlog::logger> coreLog;
		static std::shared_ptr<spdlog::logger> gameLog;
	};
}


#define GE_ENGINE_TRACE(...) ::geProject::Logging::GetEngineLog()->trace(__VA_ARGS__)
#define GE_ENGINE_INFO(...) ::geProject::Logging::GetEngineLog()->info(__VA_ARGS__)
#define GE_ENGINE_WARN(...) ::geProject::Logging::GetEngineLog()->warn(__VA_ARGS__)
#define GE_ENGINE_ERROR(...) ::geProject::Logging::GetEngineLog()->error(__VA_ARGS__)
#define GE_ENGINE_CRITICAL(...) ::geProject::Logging::GetEngineLog()->critical(__VA_ARGS__)


#define GE_GAME_TRACE(...)		::geProject::Logging::GetGameLog()->trace(__VA_ARGS__)
#define GE_GAME_INFO(...)		::geProject::Logging::GetGameLog()->info(__VA_ARGS__)
#define GE_GAME_WARN(...)		::geProject::Logging::GetGameLog()->warn(__VA_ARGS__)
#define GE_GAME_ERROR(...)		::geProject::Logging::GetGameLog()->error(__VA_ARGS__)
#define GE_GAME_CRITICAL(...)	::geProject::Logging::GetGameLog()->critical(__VA_ARGS__)