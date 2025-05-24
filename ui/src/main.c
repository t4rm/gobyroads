#define SDL_MAIN_HANDLED
// SDL 2
#include "SDL2/SDL.h"
// #include "SDL2/SDL_image.h"
#include "sdl_wrapper.h"
#include "sdl_texture_wrapper.h"
// Core game features, wrapped with UI Concepts
#include "core_wrapper.h"
// Core game features, directly from core
#include "gamestate.h"
#include "player.h"
#include "effect.h"
#include "car.h"
// Standard
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (SDLW_Initialize(&window, &renderer, WIDTH, HEIGHT) != 0)
        exit(EXIT_FAILURE);

    TextureCollection *textures = SDLW_InitTextures(renderer);

    UIGameState *uiGs = initUIGameState(ROWS, COLS);
    // Plutôt utiliser les vraies dimensions du coup.
    // Et appliquer un coefficient de réduction pour le comportement du jeu en CLI ?
    // Comment gérer le fait qu'une coordonnée est representé en 1 pixel ici.
    SDL_Event event;
    const int FPS = 12;
    const int frameTime = 1000 / FPS;

    while (uiGs->running)
    {
        Uint64 frameStartTime = SDL_GetTicks64();

        if (SDL_PollEvent(&event))
        {
            // Actions joueur
            handleEvents(uiGs, &event);
        }
        // Est-ce qu'on "fire" des évènements pour les mises à jours des voitures et on migre leur "handling" en haut ? :
        updateCars(uiGs->core);
        updateEffects(uiGs->core);
        // Fin des possibles évents à "fire" ----

        // Calcul interactions (collisions)
        // handleCollision(uiGs->core);

        // maj état du jeu (états mobs, joueur, score)
        // A faire ici : Calcul de vélocité pour les animations des obstacles ?
        scrolling(uiGs->core);

        // maj rendu & rendu
        updateGameState(uiGs->core);
        SDLW_UpdateAndRender(uiGs, renderer, textures);

        Uint64 frameEndTime = SDL_GetTicks64();
        Uint64 elapsedTime = frameEndTime - frameStartTime;

        if (elapsedTime < frameTime)
        {
            Uint64 sleepTime = frameTime - elapsedTime;
            SDL_Delay(sleepTime);
        }
    }

    SDLW_DestroyTextures(textures);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}