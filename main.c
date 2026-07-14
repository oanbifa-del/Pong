#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL_ttf.h>
#include "intro.h"

// Configurações da janela
#define SCREEN_WIDTH 1840
#define SCREEN_HEIGHT 1000

// Velocidade da bola
int ballVelX, ballVelY;

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

// Processa eventos da SDL e o movimento das raquetes
void processInput(bool *running, SDL_Rect *paddle1, SDL_Rect *paddle2)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            *running = false;
    }

    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    // Jogador 1 (W/S)
    if (keystate[SDL_SCANCODE_W] && paddle1->y > 0)
        paddle1->y -= 5;

    if (keystate[SDL_SCANCODE_S] &&
        paddle1->y < SCREEN_HEIGHT - paddle1->h)
        paddle1->y += 5;

    // Jogador 2 (Setas)
    if (keystate[SDL_SCANCODE_UP] && paddle2->y > 0)
        paddle2->y -= 5;

    if (keystate[SDL_SCANCODE_DOWN] &&
        paddle2->y < SCREEN_HEIGHT - paddle2->h)
        paddle2->y += 5;
}

// Atualiza toda a lógica do jogo
void updateGame(SDL_Rect *ball,
                SDL_Rect *paddle1,
                SDL_Rect *paddle2)
{
    // Move a bola
    ball->x += ballVelX;
    ball->y += ballVelY;

    // Rebote nas bordas superior e inferior
    if (ball->y <= 0 ||
        ball->y + ball->h >= SCREEN_HEIGHT)
    {
        ballVelY = -ballVelY;
    }

    // Colisão com as raquetes
    if (SDL_HasIntersection(ball, paddle1) ||
        SDL_HasIntersection(ball, paddle2))
    {
        ballVelX = -ballVelX;
    }

    // Saiu da tela? Reinicia a bola
    if (ball->x < 0 ||
        ball->x > SCREEN_WIDTH)
    {
        resetBall(ball);
    }
}

// Desenha todos os elementos da partida
void renderGame(SDL_Renderer *renderer,
                SDL_Rect *ball,
                SDL_Rect *paddle1,
                SDL_Rect *paddle2)
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
    SDL_Rect bordaEsquerda = {0, 0, 10, SCREEN_HEIGHT};
    SDL_Rect bordaDireita = {SCREEN_WIDTH - 10, 0, 10, SCREEN_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &bordaEsquerda);
    SDL_RenderFillRect(renderer, &bordaDireita);

    // Atualiza a tela
    SDL_RenderPresent(renderer);
}

// =====================================================
// Programa principal
// =====================================================

int main()
{
    srand(time(NULL));

    // Inicializa a SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Erro ao inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }
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
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);

    // Cria o renderizador
    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED |
            SDL_RENDERER_PRESENTVSYNC);
    // Mostra a Intro
    showIntro(renderer);

    // Objetos do jogo
    SDL_Rect paddle1 = {50, SCREEN_HEIGHT / 2 - 50, 10, 100};
    SDL_Rect paddle2 = {SCREEN_WIDTH - 60, SCREEN_HEIGHT / 2 - 50, 10, 100};

    SDL_Rect ball;

    resetBall(&ball);

    bool running = true;

    // =================================================
    // Loop principal
    // =================================================

    while (running)
    {
        processInput(&running, &paddle1, &paddle2);
        updateGame(&ball, &paddle1, &paddle2);
        renderGame(renderer, &ball, &paddle1, &paddle2);
        SDL_Delay(1000 / 60);
    }

    // Liberação dos recursos
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();
    return 0;
}
