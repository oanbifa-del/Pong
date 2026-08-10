#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL_mixer.h>

void initAudio();

void playIntroSound();

void playPaddle1Sound();
void playPaddle2Sound();
void playWallSound();

void playPongMusic();

void closeAudio();

#endif
