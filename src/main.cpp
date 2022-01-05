#include "speaker.h"
#include "utility/logger.h"
#include "version.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_sinks.h>

std::vector<spdlog::sink_ptr> setupLogger(std::string path);

int main() {

#ifdef WIN32
    SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);
    SetConsoleOutputCP(CP_UTF8);
#endif

    

    auto sinks = setupLogger("../logs/log.txt");

    spdlog::info("App version: {}", app::version);

    SDL_Init(SDL_INIT_AUDIO);

    // the representation of our audio device in SDL:
    std::shared_ptr<Speaker> speaker = std::make_shared<Speaker>();

    // open audio device
    speaker->openAudioDevice();

    // play some sound
    speaker->playSomething();

    SDL_Quit();

    return 0;
}

std::vector<spdlog::sink_ptr> setupLogger(std::string path)
{
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stderr_sink_mt>());
    sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>(path, 23, 59));

    auto logger = utility::setup_logger(sinks);
    auto lvl = spdlog::level::level_enum::debug;
    spdlog::set_level(lvl);

    return sinks;
}