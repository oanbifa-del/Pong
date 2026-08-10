#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL_ttf.h>
#include "audio.h"
#include "intro.h"
#include "menu.h"
#include <SDL2/SDL_image.h>

// Configurações da janela
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1000
#define PLAYFIELD_MARGIN_X 80
#define PLAYFIELD_MARGIN_Y 30

// velocidade da raquete
#define PADDLE_SPEED 12

// Velocidade da bola
int ballVelX, ballVelY;
// Velocidade da raquete
int paddle1Velocity = 0;
int paddle2Velocity = 0;

// =====================================================
//      Funções do jogo
// =====================================================

//Mostra a Introdução
void showIntro(SDL_Renderer *renderer);

// Reinicia a bola no centro da tela e sorteia uma nova direção inicial.
void resetBall(SDL_Rect *ball)
{
    ball->x = SCREEN_WIDTH / 2 - 10;
    ball->y = SCREEN_HEIGHT / 2 - 10;
    ball->w = 20;
    ball->h = 20;

    ballVelX = (rand() % 2) ? 15 : -15;
    ballVelY = (rand() % 7) - 4;
}

// Atualiza toda a lógica do jogo
void updateGame(SDL_Rect *ball,
                SDL_Rect *paddle1,
                SDL_Rect *paddle2,
                int paddle1Velocity,
                int paddle2Velocity)
{
    // Move a bola
    ball->x += ballVelX;
    ball->y += ballVelY;

    // Rebote/colisao nas linhas verde da arena
    if (ball->y <= PLAYFIELD_MARGIN_Y ||
        ball->y + ball->h >= SCREEN_HEIGHT - PLAYFIELD_MARGIN_Y)
    {
        playWallSound();
        ballVelY = -ballVelY;
    }

    // Colisão com as raquetes
    if (SDL_HasIntersection(ball, paddle1))
    {
        playPaddle1Sound();
        ballVelX = -ballVelX;
        ballVelY += paddle1Velocity / 2;
    }

    if (SDL_HasIntersection(ball, paddle2))
    {
        playPaddle2Sound();
        ballVelX = -ballVelX;
        ballVelY += paddle2Velocity / 2;
    }

}
// Processa eventos da SDL e o movimento das raquetes
void processInput(bool *running,
                    SDL_Window *window,
                    SDL_Renderer *renderer,
                    bool *fullscreen,
                    SDL_Rect *paddle1,
                    SDL_Rect *paddle2,
                    SDL_Rect *ball,
                    int *score1,
                    int *score2,
                    int *paddle1Velocity,
                    int *paddle2Velocity, bool *paused)
                    {
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        // Fecha o jogo
        if (event.type == SDL_QUIT)
            *running = false;

        // Tecla ESC encerra o jogo
        if (event.type == SDL_KEYDOWN &&
            event.key.keysym.sym == SDLK_ESCAPE)
        {
            *running = false;
        }

        // F1 volta para a tela de preparação
        if (event.type == SDL_KEYDOWN &&
            event.key.keysym.sym == SDLK_F1)
        {
            resetBall(ball);

            *score1 = 0;
            *score2 = 0;

            Mix_HaltMusic(); // para a música do jogo

            showMenu(renderer); // volta para tela de preparação
            playPongMusic();    // reinicia a música ao voltar ao jogo
        }

        // Espaço pausa/despausa o jogo
        if (event.type == SDL_KEYDOWN &&
            event.key.keysym.sym == SDLK_SPACE)
        {
            *paused = !(*paused);
        }

        // Alterna entre janela e tela cheia
        if (event.type == SDL_KEYDOWN &&
            event.key.keysym.sym == SDLK_F11)
        {
            *fullscreen = !(*fullscreen);

            SDL_SetWindowFullscreen(
                window,
                *fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0
            );
        }
    }

    // Assume que as raquetes estão paradas neste frame
    *paddle1Velocity = 0;
    *paddle2Velocity = 0;
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    // Jogador 1 (W/S)
    if (keystate[SDL_SCANCODE_W] &&
        paddle1->y > PLAYFIELD_MARGIN_Y)
    {
        paddle1->y -= PADDLE_SPEED;
        *paddle1Velocity = -PADDLE_SPEED;
    }

    if (keystate[SDL_SCANCODE_S] &&
        paddle1->y < SCREEN_HEIGHT - PLAYFIELD_MARGIN_Y - paddle1->h)
    {
        paddle1->y += PADDLE_SPEED;
        *paddle1Velocity = PADDLE_SPEED;
    }

    // Jogador 2 (Setas)
    if (keystate[SDL_SCANCODE_UP] &&
        paddle2->y > PLAYFIELD_MARGIN_Y)
    {
        paddle2->y -= PADDLE_SPEED;
        *paddle2Velocity = -PADDLE_SPEED;
    }

    if (keystate[SDL_SCANCODE_DOWN] &&
        paddle2->y < SCREEN_HEIGHT - PLAYFIELD_MARGIN_Y - paddle2->h)
    {
        paddle2->y += PADDLE_SPEED;
        *paddle2Velocity = PADDLE_SPEED;
    }

}

// Atualiza o Placar
void updateScore(SDL_Rect *ball, int *score1, int *score2)
{
    if (ball->x < PLAYFIELD_MARGIN_X)
    {
        (*score2)++;
        resetBall(ball);
    }

    if (ball->x + ball->w > SCREEN_WIDTH - PLAYFIELD_MARGIN_X)
    {
        (*score1)++;
        resetBall(ball);
    }
}

// Renderiza o placar no topo da tela
void renderScore(SDL_Renderer *renderer, int score1, int score2)
{
    TTF_Font *font = TTF_OpenFont(
        "assets/fonts/Orbitron-Bold.ttf",
        70
    );

    if (!font)
    {
        printf("Erro ao carregar fonte do placar: %s\n", TTF_GetError());
        return;
    }

    char textoPlacar[20];

    // Junta os dois valores em uma string
    sprintf(textoPlacar, "%d    :    %d", score1, score2);

    SDL_Color cor = {0, 255, 127, 255};

    SDL_Surface *surface =
        TTF_RenderText_Blended(
            font,
            textoPlacar,
            cor
        );

    SDL_Texture *texture =
        SDL_CreateTextureFromSurface(
            renderer,
            surface
        );

    SDL_Rect destino;

    destino.w = surface->w;
    destino.h = surface->h;

    // Centraliza horizontalmente
    destino.x = SCREEN_WIDTH / 2 - destino.w / 2;

    // Posição vertical do placar
    destino.y = 40;

    SDL_RenderCopy(
        renderer,
        texture,
        NULL,
        &destino
    );

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
}
// Desenha todos os elementos da partida
void renderGame(SDL_Renderer *renderer,
                SDL_Rect *ball,
                SDL_Rect *paddle1,
                SDL_Rect *paddle2,
                int score1,
                int score2)
{
    // Limpa a tela
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Desenha as raquetes
    SDL_SetRenderDrawColor(renderer, 100, 149, 237, 255);
    SDL_RenderFillRect(renderer, paddle1);
    SDL_RenderFillRect(renderer, paddle2);

    // Desenha a bola
    filledCircleRGBA(
        renderer,
        ball->x + ball->w / 2,
        ball->y + ball->h / 2,
        ball->w / 2,
        255, 255, 255, 255);

    // Bordas laterais
    SDL_Rect bordaEsquerda = {
        PLAYFIELD_MARGIN_X,
        PLAYFIELD_MARGIN_Y,
        10,
        SCREEN_HEIGHT - 2 * PLAYFIELD_MARGIN_Y
    };

    SDL_Rect bordaDireita = {
        SCREEN_WIDTH - PLAYFIELD_MARGIN_X - 10,
        PLAYFIELD_MARGIN_Y,
        10,
        SCREEN_HEIGHT - 2 * PLAYFIELD_MARGIN_Y
    };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &bordaEsquerda);
    SDL_RenderFillRect(renderer, &bordaDireita);
    renderScore(renderer, score1, score2);
    SDL_SetRenderDrawColor(renderer, 57, 255, 20, 255); // Verde neon
    // Desenha as margens superior e Inferior
    SDL_RenderDrawLine(
        renderer,
        PLAYFIELD_MARGIN_X,
        PLAYFIELD_MARGIN_Y,
        SCREEN_WIDTH - PLAYFIELD_MARGIN_X,
        PLAYFIELD_MARGIN_Y
    );

    SDL_RenderDrawLine(
        renderer,
        PLAYFIELD_MARGIN_X,
        SCREEN_HEIGHT - PLAYFIELD_MARGIN_Y,
        SCREEN_WIDTH - PLAYFIELD_MARGIN_X,
        SCREEN_HEIGHT - PLAYFIELD_MARGIN_Y
    );


    // Atualiza a tela
    SDL_RenderPresent(renderer);
}

// =====================================================
//                Programa principal
// =====================================================
int main()
{
    srand(time(NULL));
    bool fullscreen = false;
    bool paused = false;

    // Inicializa a SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Erro ao inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }
    //Incializa imagem do logo
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        printf("Erro ao inicializar SDL_image: %s\n", IMG_GetError());
        return 1;
    }

    // Inicia o audio
    initAudio();
    // Inicializa o TTF
    if (TTF_Init() == -1)
    {
        printf("Erro ao inicializar SDL_ttf: %s\n", TTF_GetError());
        return 1;
    }

    // Cria a janela
    SDL_Window *window = SDL_CreateWindow(
        "Pong",
        SDL_WINDOWPOS_CENTERED,
        0,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);

    // Cria o renderizador
    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED |
            SDL_RENDERER_PRESENTVSYNC);
    // Toca o som e mostra a Intro
    playIntroSound();
    showIntro(renderer);
    // Mostra o Menu
    showMenu(renderer);

    // Objetos do jogo
    SDL_Rect paddle1 = {
        PLAYFIELD_MARGIN_X + 40,
        SCREEN_HEIGHT / 2 - 50,
        10,
        100
    };

    SDL_Rect paddle2 = {
        SCREEN_WIDTH - PLAYFIELD_MARGIN_X - 50,
        SCREEN_HEIGHT / 2 - 50,
        10,
        100
    };

    SDL_Rect ball;

    // Placares
    int score1 = 0;
    int score2 = 0;

    resetBall(&ball);

    bool running = true;

    playPongMusic();

    // =================================================
    // Loop principal
    // =================================================

    while (running)
    {
        processInput(&running, window, renderer, &fullscreen,
                     &paddle1, &paddle2, &ball, &score1, &score2,
                     &paddle1Velocity, &paddle2Velocity, &paused);

        if (!paused)
        {
            updateGame(&ball, &paddle1, &paddle2,
                       paddle1Velocity, paddle2Velocity);

            updateScore(&ball, &score1, &score2);
        }

        renderGame(renderer, &ball, &paddle1, &paddle2, score1, score2);
        SDL_Delay(1000 / 60);
    }

    // Liberação dos recursos
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    closeAudio();
    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
    return 0;
}
