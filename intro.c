#include "intro.h"

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 1840
#define SCREEN_HEIGHT 1000

void showIntro(SDL_Renderer *renderer)
{
    TTF_Font *font =
        TTF_OpenFont("assets/fonts/Orbitron-Bold.ttf", 110);

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

    SDL_Rect pongRect={
        -surfPong->w,
        SCREEN_HEIGHT/2-120,
        surfPong->w,
        surfPong->h
    };

    SDL_Rect anoRect={
        SCREEN_WIDTH,
        SCREEN_HEIGHT/2,
        surf2026->w,
        surf2026->h
    };

    int destinoPong =
        SCREEN_WIDTH/2 - surfPong->w/2 - 10;

    int destinoAno =
        SCREEN_WIDTH/2 - surf2026->w/2 + 9;

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


    SDL_Delay(5000);


    // Fade out
    for(int alpha=255; alpha>=0; alpha-=5)
    {
        SDL_Event e;

        while(SDL_PollEvent(&e))
            if(e.type==SDL_QUIT)
                return;

        SDL_SetTextureAlphaMod(texPong,alpha);
        SDL_SetTextureAlphaMod(tex2026,alpha);

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer,texPong,NULL,&pongRect);
        SDL_RenderCopy(renderer,tex2026,NULL,&anoRect);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyTexture(texPong);
    SDL_DestroyTexture(tex2026);

    SDL_FreeSurface(surfPong);
    SDL_FreeSurface(surf2026);

    TTF_CloseFont(font);
}
