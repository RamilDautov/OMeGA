#ifndef OMEGA_LOGGER
#define OMEGA_LOGGER

#include <spdlog/logger.h>

namespace utility
{
	std::shared_ptr<spdlog::logger> setup_logger(const std::vector<spdlog::sink_ptr>&);
	void shutdown_logger();
}


#endif // !OMEGA_LOGGER