#include "intro.h"

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1000

void showIntro(SDL_Renderer *renderer)
{
    TTF_Font *font =
        TTF_OpenFont("assets/fonts/Orbitron-Bold.ttf", 120);

    if (!font)
    {
        printf("Erro ao carregar fonte: %s\n", TTF_GetError());
        return;
    }

    SDL_Color white = {255,255,255,255};

    SDL_Surface *surfPong =
        TTF_RenderUTF8_Blended(font,"PONG",white);

    SDL_Surface *surf2026 =
        TTF_RenderUTF8_Blended(font,"2026",white);

    SDL_Texture *texPong =
        SDL_CreateTextureFromSurface(renderer,surfPong);

    SDL_Texture *tex2026 =
        SDL_CreateTextureFromSurface(renderer,surf2026);

    SDL_Texture *logo = IMG_LoadTexture(renderer, "assets/images/pong_logo.png");

    if (!logo)
    {
        printf("Erro ao carregar logo: %s\n", IMG_GetError());
        return;
    }

    int logoW, logoH;
    SDL_QueryTexture(logo, NULL, NULL, &logoW, &logoH);

    SDL_Rect logoRect = {
        SCREEN_WIDTH / 2 - logoW / 2,
        SCREEN_HEIGHT / 2 - logoH / 2 - 50,
        logoW,
        logoH
    };

    SDL_Rect pongRect={
        -surfPong->w,
        SCREEN_HEIGHT/2-200,
        surfPong->w,
        surfPong->h
    };

    SDL_Rect anoRect={
        SCREEN_WIDTH,
        SCREEN_HEIGHT/2-100,
        surf2026->w,
        surf2026->h
    };

    int destinoPong =
        SCREEN_WIDTH/2 - surfPong->w/2 - 2;

    int destinoAno =
        SCREEN_WIDTH/2 - surf2026->w/2 + 16;

    // Entrada
    while(pongRect.x < destinoPong ||
          anoRect.x > destinoAno)
    {
        SDL_Event e;

        while(SDL_PollEvent(&e))
            if(e.type==SDL_QUIT)
                return;

        if(pongRect.x < destinoPong)
            pongRect.x += 12;

        if(anoRect.x > destinoAno)
            anoRect.x -= 12;

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer,texPong,NULL,&pongRect);
        SDL_RenderCopy(renderer,tex2026,NULL,&anoRect);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }
        // Muda a cor do texto Pong
        SDL_SetTextureColorMod(texPong, 255, 165, 0);
        SDL_SetTextureColorMod(tex2026, 5, 165, 255);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, texPong, NULL, &pongRect);
        SDL_RenderCopy(renderer, tex2026, NULL, &anoRect);

        SDL_RenderPresent(renderer);

    SDL_Delay(1500);

    // Zoom do logo
    for (int escala = 100; escala <= 250; escala += 5)
    {
        SDL_Rect zoom;

        zoom.w = logoRect.w * escala / 100;
        zoom.h = logoRect.h * escala / 100;

        zoom.x = SCREEN_WIDTH / 2 - zoom.w / 2;
        zoom.y = SCREEN_HEIGHT / 2 - zoom.h / 2 - 50;

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, logo, NULL, &zoom);

        SDL_RenderPresent(renderer);

        SDL_Delay(20);
    }

    SDL_DestroyTexture(logo);
    SDL_DestroyTexture(texPong);
    SDL_DestroyTexture(tex2026);

    SDL_FreeSurface(surfPong);
    SDL_FreeSurface(surf2026);

    TTF_CloseFont(font);
}
