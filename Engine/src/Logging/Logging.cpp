#include <ge_engine/Logging.h>

namespace geProject {
	std::shared_ptr<spdlog::logger> Logging::coreLog;
	std::shared_ptr<spdlog::logger> Logging::gameLog;

	void Logging::Init() {
		spdlog::set_pattern("%^[%T] %n: %v%$");
		coreLog = spdlog::stdout_color_mt("Engine");
		coreLog->set_level(spdlog::level::trace);
		gameLog = spdlog::stdout_color_mt("Game");
		gameLog->set_level(spdlog::level::trace);
	}
}