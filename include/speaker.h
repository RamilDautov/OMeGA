#ifndef OMEGA_SPEAKER
#define OMEGA_SPEAKER
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>

class Speaker
{
public:
	Speaker() {};
	~Speaker();
	void openAudioDevice();
	void playMidiFile(const std::string& path);

private:
	SDL_AudioDeviceID m_audioDevice{0};
};



#endif // !OMEGA_SPEAKER
