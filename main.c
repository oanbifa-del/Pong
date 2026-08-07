#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL_ttf.h>
#include "intro.h"
#include "menu.h"

// Configurações da janela
#define SCREEN_WIDTH 1840
#define SCREEN_HEIGHT 1000
// velocidade da raquete
#define PADDLE_SPEED 10

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

    // Rebote nas bordas superior e inferior
    if (ball->y <= 0 ||
        ball->y + ball->h >= SCREEN_HEIGHT)
    {
        ballVelY = -ballVelY;
    }

    // Colisão com as raquetes
    if (SDL_HasIntersection(ball, paddle1))
    {
        ballVelX = -ballVelX;
        ballVelY += paddle1Velocity / 2;
    }

    if (SDL_HasIntersection(ball, paddle2))
    {
        ballVelX = -ballVelX;
        ballVelY += paddle2Velocity / 2;
    }
}
// Processa eventos da SDL e o movimento das raquetes
void processInput(bool *running,
                   SDL_Rect *paddle1,
                   SDL_Rect *paddle2,
                   int *paddle1Velocity,
                   int *paddle2Velocity)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            *running = false;
    }

    // Assume que as raquetes estão paradas neste frame
    *paddle1Velocity = 0;
    *paddle2Velocity = 0;
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    // Jogador 1 (W/S)
    if (keystate[SDL_SCANCODE_W] && paddle1->y > 0)
    {
        paddle1->y -= PADDLE_SPEED;
        *paddle1Velocity = -PADDLE_SPEED;
    }

    if (keystate[SDL_SCANCODE_S] &&
        paddle1->y < SCREEN_HEIGHT - paddle1->h)
    {
        paddle1->y += PADDLE_SPEED;
        *paddle1Velocity = PADDLE_SPEED;
    }

    // Jogador 2 (Setas)
    if (keystate[SDL_SCANCODE_UP] && paddle2->y > 0)
    {
        paddle2->y -= PADDLE_SPEED;
        *paddle2Velocity = -PADDLE_SPEED;
    }
    if (keystate[SDL_SCANCODE_DOWN] &&
        paddle2->y < SCREEN_HEIGHT - paddle2->h)
    {
        paddle2->y += PADDLE_SPEED;
        *paddle2Velocity = PADDLE_SPEED;
    }
}

// Atualiza o Placar
void updateScore(SDL_Rect *ball, int *score1, int *score2)
{
    if (ball->x < 0)
    {
        (*score2)++;
        resetBall(ball);
    }

    if (ball->x > SCREEN_WIDTH)
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
    SDL_Rect bordaEsquerda = {0, 0, 10, SCREEN_HEIGHT};
    SDL_Rect bordaDireita = {SCREEN_WIDTH - 10, 0, 10, SCREEN_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &bordaEsquerda);
    SDL_RenderFillRect(renderer, &bordaDireita);
    renderScore(renderer, score1, score2);
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
    // Mostra o Menu
    showMenu(renderer);

    // Objetos do jogo
    SDL_Rect paddle1 = {50, SCREEN_HEIGHT / 2 - 50, 10, 100};
    SDL_Rect paddle2 = {SCREEN_WIDTH - 60, SCREEN_HEIGHT / 2 - 50, 10, 100};

    SDL_Rect ball;

    // Placares
    int score1 = 0;
    int score2 = 0;

    resetBall(&ball);

    bool running = true;

    // =================================================
    // Loop principal
    // =================================================

    while (running)
    {
        processInput(&running, &paddle1, &paddle2, &paddle1Velocity, &paddle2Velocity);
        updateGame(&ball, &paddle1, &paddle2, paddle1Velocity, paddle2Velocity);
        updateScore(&ball, &score1, &score2);
        renderGame(renderer, &ball, &paddle1, &paddle2, score1, score2);
        SDL_Delay(1000 / 60);
    }

    // Liberação dos recursos
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();
    return 0;
}
