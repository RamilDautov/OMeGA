#ifndef OMEGA_SPEAKER
#define OMEGA_SPEAKER
#include <SDL2/SDL.h>


class Speaker
{
public:
	Speaker() {};
	~Speaker();
	void openAudioDevice();
	void playSomething();

private:
	SDL_AudioDeviceID m_audioDevice;
};



#endif // !OMEGA_SPEAKER
