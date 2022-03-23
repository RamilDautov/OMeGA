#include "logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#ifdef __APPLE__
#include <syslog.h>
#endif
#if defined __linux__
#include <execinfo.h>
#include <sys/time.h>
#include <X11/Xlib.h>
#endif

std::shared_ptr<spdlog::logger> utility::setup_logger(const std::vector<spdlog::sink_ptr>& sinks)
{
	constexpr auto logger_name = "core";
	auto logger = spdlog::get(logger_name);
	if (!logger)
	{
		if (sinks.size() > 0)
		{
			logger = std::make_shared<spdlog::logger>(logger_name, std::begin(sinks), std::end(sinks));
		}
		else
		{
			logger = spdlog::stdout_color_mt(logger_name);
		}
		spdlog::register_logger(logger);
		spdlog::set_default_logger(logger);
		spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%t] [%n] [%l] %v", spdlog::pattern_time_type::utc);
		spdlog::flush_every(std::chrono::seconds(5));
	}
	return logger;
}

void utility::shutdown_logger()
{
	spdlog::shutdown();
}