#include "speaker.h"
#include "midi.h"
#include "utility/logger.h"
#include "version.h"
#include "algorithm/genetic.h"
#include <iostream>
#include <SDL2/SDL.h>
#include "SDL2/SDL_mixer.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <thread>

using namespace GeneticAlgorithm;

std::vector<spdlog::sink_ptr> setupLogger(std::string path);
int fitnessFunc(Genome genome);

int main() {

#ifdef WIN32
    SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);
    SetConsoleOutputCP(CP_UTF8);
#endif

    

    auto sinks = setupLogger("../logs/log.txt");

    spdlog::info("App version: {}", app::version);

    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
    Mix_VolumeMusic(100);

    // the representation of our audio device in SDL:
    std::shared_ptr<Speaker> speaker = std::make_shared<Speaker>();

    // open audio device
    speaker->openAudioDevice();

    // play some sound
    //speaker->playSomething();

    //Scale scale(ScaleType::MAJOR, "C");
    Scale scale(ScaleType::MINOR_BLUES, "C");
    MidiEncoder midiEncoder(scale, 130);

    auto pop = Genetic::runEvolution(fitnessFunc, 5, 10);

    midiEncoder.encodeGenomeToMidi(pop[0], "untitled.mid");

    spdlog::info("Midi encoded to file");
    std::this_thread::sleep_for(std::chrono::seconds(3));
    //Load song
    std::string midiFile = "untitled.mid";
    Mix_Music* song = Mix_LoadMUS(midiFile.c_str());

    //Play song
    Mix_PlayMusic(song, 1);

    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));

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

int fitnessFunc(Genome genome)
{
    int res;
    std::cout << "Enter the number: ";
    std::cin >> res;
    std::cout << std::endl;

    return res;
}