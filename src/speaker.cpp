#include "speaker.h"
#include <fmt/ostream.h>

#define SAMPLING_FREQ 44100
#define AUDIO_BUFFER_SIZE 1024
#define MONO 1
#define STEREO 2

Speaker::~Speaker()
{
    SDL_CloseAudioDevice(m_audioDevice);
}

void Speaker::openAudioDevice()
{
    SDL_AudioSpec audio_spec;
    SDL_zero(audio_spec);
    audio_spec.freq = SAMPLING_FREQ;
    audio_spec.format = AUDIO_S16SYS;
    audio_spec.channels = MONO;
    audio_spec.samples = AUDIO_BUFFER_SIZE;
    audio_spec.callback = NULL;

    m_audioDevice = SDL_OpenAudioDevice(NULL, 0, &audio_spec, NULL, 0);

    if (m_audioDevice == 0)
        throw std::runtime_error(fmt::format("Speaker error: {}", SDL_GetError()));
}

void Speaker::playSomething()
{
    // pushing 3 seconds of samples to the audio buffer:
    float x = 0;
    for (int i = 0; i < SAMPLING_FREQ * 3; i++) {
        x += .010f;

        // SDL_QueueAudio expects a signed 16-bit value
        // note: "5000" here is just gain so that we will hear something
        int16_t sample = sin(x * 4) * 5000;

        const int sample_size = sizeof(int16_t) * 1;
        SDL_QueueAudio(m_audioDevice, &sample, sample_size);
    }

    // unpausing the audio device (starts playing):
    SDL_PauseAudioDevice(m_audioDevice, 0);

    SDL_Delay(3000);
}