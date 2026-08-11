#include "audio.h"
#include <SDL2/SDL_mixer.h>
#include <stdio.h>

// Áudios carregados pelo sistema
static Mix_Chunk *introSound = NULL;

// Música de fundo
static Mix_Music *pongMusic = NULL;

// Sons das colisões
static Mix_Chunk *paddle1Sound = NULL; // Nota Dó - raquete 1
static Mix_Chunk *paddle2Sound = NULL; // Nota Ré - raquete 2
static Mix_Chunk *wallSound = NULL;    // Nota Mi - parede
static Mix_Chunk *score1Sound = NULL;
static Mix_Chunk *score2Sound = NULL;

// Inicializa o sistema de áudio e carrega os arquivos
void initAudio()
{
    if (Mix_OpenAudio(44100,
                      MIX_DEFAULT_FORMAT,
                      2,
                      2048) < 0)
    {
        printf("Erro ao inicializar áudio: %s\n",
               Mix_GetError());
        return;
    }

    // Vinheta da introdução
    introSound = Mix_LoadWAV(
        "assets/sounds/intro.wav"
    );

    // Música de fundo
    pongMusic = Mix_LoadMUS(
        "assets/sounds/pongMusic.mp3"
    );

    // Sons das colisões
    paddle1Sound = Mix_LoadWAV(
        "assets/sounds/xilofonC.wav"
    );

    paddle2Sound = Mix_LoadWAV(
        "assets/sounds/xilofonD.wav"
    );

    wallSound = Mix_LoadWAV(
        "assets/sounds/xilofonE.wav"
    );

    score1Sound = Mix_LoadWAV(
        "assets/sounds/scoreC.wav"
    );

    score2Sound = Mix_LoadWAV(
        "assets/sounds/scoreDm.wav"
    );

    // Verifica carregamento dos arquivos
    if (!introSound)
        printf("Erro ao carregar intro.wav: %s\n",
               Mix_GetError());
    if (!pongMusic)
        printf("Erro ao carregar pongMusic.mp3: %s\n",
               Mix_GetError());

    if (!paddle1Sound)
        printf("Erro ao carregar xilofonC.wav: %s\n",
               Mix_GetError());

    if (!paddle2Sound)
        printf("Erro ao carregar xilofonD.wav: %s\n",
               Mix_GetError());

    if (!wallSound)
        printf("Erro ao carregar xilofonE.wav: %s\n",
               Mix_GetError());
    if (!score1Sound)
        printf("Erro ao carregar xilofonE.wav: %s\n",
               Mix_GetError());
    if (!score2Sound)
        printf("Erro ao carregar xilofonE.wav: %s\n",
               Mix_GetError());
}


// Toca a vinheta da introdução
void playIntroSound()
{
    if (introSound)
    {
        Mix_PlayChannel(-1, introSound, 0);
    }
}

void playPongMusic()
{
    if (pongMusic)
    {
        Mix_PlayMusic(pongMusic, -1);
    }
}


// Toca som da raquete 1 (Dó)
void playPaddle1Sound()
{
    if (paddle1Sound)
    {
        Mix_PlayChannel(-1, paddle1Sound, 0);
    }
}


// Toca som da raquete 2 (Ré)
void playPaddle2Sound()
{
    if (paddle2Sound)
    {
        Mix_PlayChannel(-1, paddle2Sound, 0);
    }
}


// Toca som da colisão com a parede (Mi)
void playWallSound()
{
    if (wallSound)
    {
        Mix_PlayChannel(-1, wallSound, 0);
    }
}

void playScore1Sound()
{
    if (score1Sound)
    {
        Mix_PlayChannel(-1, score1Sound, 0);
    }
}

void playScore2Sound()
{
    if (score2Sound)
    {
        Mix_PlayChannel(-1, score2Sound, 0);
    }
}


// Libera memória e encerra o áudio
void closeAudio()
{
    if (introSound)
        Mix_FreeChunk(introSound);

    if (paddle1Sound)
        Mix_FreeChunk(paddle1Sound);

    if (paddle2Sound)
        Mix_FreeChunk(paddle2Sound);

    if (wallSound)
        Mix_FreeChunk(wallSound);

    if (score1Sound)
        Mix_FreeChunk(score1Sound);

    if (score2Sound)
        Mix_FreeChunk(score2Sound);

    Mix_CloseAudio();
}
