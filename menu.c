#include "menu.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1000


// Desenha um texto na tela
void drawText(SDL_Renderer *renderer,
              TTF_Font *font,
              const char *texto,
              int x,
              int y,
              SDL_Color cor)
{
    SDL_Surface *surface =
        TTF_RenderText_Blended(font, texto, cor);

    if (!surface)
    {
        printf("Erro ao criar texto: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *texture =
        SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect destino;

    destino.x = x;
    destino.y = y;
    destino.w = surface->w;
    destino.h = surface->h;


    SDL_RenderCopy(renderer, texture, NULL, &destino);


    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}


// Tela de preparação/menu antes do jogo
void showMenu(SDL_Renderer *renderer)
{
    TTF_Font *fontTitulo =
        TTF_OpenFont(
            "assets/fonts/Orbitron-Bold.ttf",
            70
        );

    TTF_Font *fontTexto =
        TTF_OpenFont(
            "assets/fonts/Orbitron-Bold.ttf",
            40
        );

    TTF_Font *fontTextoPequeno = TTF_OpenFont(
        "assets/fonts/Orbitron-Bold.ttf",
        35
    );

    if (!fontTitulo || !fontTexto)
    {
        printf("Erro ao carregar fonte do menu: %s\n",
               TTF_GetError());
        return;
    }

    bool esperando = true;

    SDL_Event event;

    while (esperando)
    {

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                esperando = false;
            }

            if (event.type == SDL_KEYDOWN &&
                event.key.keysym.sym == SDLK_RETURN)
            {
                esperando = false;
            }
        }

        // Fundo preto
        SDL_SetRenderDrawColor(
            renderer,
            0,
            0,
            0,
            255
        );

        SDL_RenderClear(renderer);

        // Cores da identidade visual
        SDL_Color laranja =
            {255, 165, 0, 255};

        SDL_Color verde =
            {57, 255, 20, 255};

        SDL_Color azul =
            {0, 180, 255, 255};

        SDL_Color vermelho =
            {255, 0, 10, 255};

        // Título
        drawText(
            renderer,
            fontTitulo,
            "-= PREPARE-SE =-",
            SCREEN_WIDTH/2 - 376,
            80,
            laranja
        );

        // Jogador 1
        drawText(
            renderer,
            fontTexto,
            "JOGADOR 1",
            350,
            280,
            verde
        );

        drawText(
            renderer,
            fontTexto,
            "W  -  SUBIR",
            350,
            385,
            azul
        );

        drawText(
            renderer,
            fontTexto,
            "S  -  DESCER",
            350,
            470,
            azul
        );

        // Jogador 2
        drawText(
            renderer,
            fontTexto,
            "JOGADOR 2",
            1220,
            280,
            verde
        );

        drawText(
            renderer,
            fontTexto,
            "UP    -  SUBIR",
            1220,
            385,
            azul
        );

        drawText(
            renderer,
            fontTexto,
            "DOWN  -  DESCER",
            1220,
            470,
            azul
        );
        drawText(
            renderer,
            fontTexto,
            "ESC - Sair",
            350,
            590,
            laranja
        );
        drawText(
            renderer,
            fontTexto,
            "F1 - Reset",
            350,
            660,
            laranja
        );
        drawText(
            renderer,
            fontTexto,
            "SPACE - Pausar",
            350,
            730,
            laranja
        );

        // Texto piscante do ENTER
        Uint32 tempo =
            SDL_GetTicks();

        if ((tempo / 500) % 2 == 0)
        {
            drawText(
                renderer,
                fontTextoPequeno,
                "PRESSIONE ENTER PARA COMECAR...",
                SCREEN_WIDTH/2 - 384,
                850,
                vermelho
            );
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    TTF_CloseFont(fontTitulo);
    TTF_CloseFont(fontTexto);
}
