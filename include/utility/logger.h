#ifndef OMEGA_LOGGER
#define OMEGA_LOGGER

#include <spdlog/logger.h>

namespace utility
{
	std::shared_ptr<spdlog::logger> setupLogger(const std::vector<spdlog::sink_ptr>&);
	void shutdownLogger();
}


#endif // !OMEGA_LOGGER