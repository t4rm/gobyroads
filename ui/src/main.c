#define SDL_MAIN_HANDLED

// SDL 2
#include "SDL2/SDL.h"
// #include "SDL2/SDL_image.h"
#include "sdl_wrapper.h"
#include "sdl_texture.h"
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

// Windows dimension constants
const int WIDTH = 15 * 48;
const int HEIGHT = 20 * 48;

int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (SDLW_Initialize(&window, &renderer, WIDTH, HEIGHT) != 0)
        exit(EXIT_FAILURE);

    Textures *textures = initTextures(renderer);

    UIGameState *uiGs = initUIGameState(15, 32 - 12);
    // Plutôt utiliser les vraies dimensions du coup.
    // Et appliquer un coefficient de réduction pour le comportement du jeu en CLI ?
    // Comment gérer le fait qu'une coordonnée est representé en 1 pixel ici.

    SDL_Event event;

    while (uiGs->running)
    {
        Uint64 start = SDL_GetPerformanceCounter();

        if (SDL_PollEvent(&event))
        {
            // actions joueur
            handleEvents(uiGs, &event);
            // actions des mobs
            // calcul interactions (collisions)
            // maj état du jeu (états mobs, joueur, score)
            // maj rendu & rendu
            SDLW_UpdateAndRender(uiGs, renderer, textures);
        }

        // Physics loop
        // Uint32 current = SDL_GetTicks();

        // Calculate dT (in seconds)

        // float dT = (current - lastUpdate) / 1000.0f;
        // for (/* objects */)
        // {
        //     object.position += object.velocity * dT;
        // }

        // // Set updated time
        // lastUpdate = current;

        // 60 FPS CAP
        Uint64 end = SDL_GetPerformanceCounter();

        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

        float delayTime = 16.666f - elapsedMS;
        if (delayTime > 0)
            SDL_Delay((Uint32)floor(delayTime));
    }

    destroyTextures(textures);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}