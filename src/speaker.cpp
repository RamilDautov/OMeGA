#include "speaker.h"
#include <fmt/ostream.h>
#include <sstream>

static constexpr int SAMPLING_FREQ = 44100;
static constexpr int AUDIO_BUFFER_SIZE = 1024;
static constexpr int MONO			   = 1;
static constexpr int STEREO			   = 2;

Speaker::~Speaker()
{
    SDL_CloseAudioDevice(m_audioDevice);
}

void Speaker::openAudioDevice()
{
	SDL_Init(SDL_INIT_AUDIO);

	SDL_AudioSpec audio_spec;
	SDL_zero(audio_spec);
	audio_spec.freq		= SAMPLING_FREQ;
	audio_spec.format	= AUDIO_S16SYS;
	audio_spec.channels = MONO;
	audio_spec.samples	= AUDIO_BUFFER_SIZE;
	audio_spec.callback = NULL;

	m_audioDevice = SDL_OpenAudioDevice(NULL, 0, &audio_spec, NULL, 0);
	Mix_OpenAudio(44100, AUDIO_U8, 2, 1024);
	Mix_VolumeMusic(100);

    if (m_audioDevice == 0)
        throw std::runtime_error(fmt::format("Speaker error: {}", SDL_GetError()));
}

void Speaker::playMidiFile(const std::string& path)
{
	Mix_Music* song	= Mix_LoadMUS(path.c_str());

	// Play song
	Mix_PlayMusic(song, 1);
}