#include "speaker.h"
#include <iostream>
#include <SDL2/SDL.h>

int main() {

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