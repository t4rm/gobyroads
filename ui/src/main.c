#define SDL_MAIN_HANDLED
// SDL 2
#include "SDL2/SDL.h"
#include <SDL2/SDL_mixer.h>
// #include "SDL2/SDL_image.h"
#include "sdl_wrapper.h"
#include "sdl_texture_wrapper.h"
#include "sdl_screens_wrapper.h"
#include "sdl_mixer_wrapper.h"
// Core game features, wrapped with UI Concepts
#include "core_wrapper.h"
// Core game features, directly from core
#include "gamestate.h"
#include "player.h"
#include "car.h"
// Standard
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Fonts *fonts = (TTF_Fonts *)malloc(sizeof(TTF_Fonts));

    if (SDLW_Initialize(&window, &renderer, &fonts, WIDTH, HEIGHT) != 0)
        exit(EXIT_FAILURE);

    TextureCollection *textures = SDLW_InitTextures(renderer);
    AudioCollection *audio = SDLW_InitAudio();
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);

    UIGameState *uiGs = initUIGameState(ROWS, COLS);
    // Plutôt utiliser les vraies dimensions du coup.
    // Et appliquer un coefficient de réduction pour le comportement du jeu en CLI ?
    // Comment gérer le fait qu'une coordonnée est representé en 1 pixel ici.
    SDL_Event event;
    const int FPS = 12;
    const int frameTime = 1000 / FPS;
    bool playedGameOverMusic = false;
    bool playedBgMusic = false;

    while (uiGs->running)
    {
        Uint64 frameStartTime = SDL_GetTicks64();

        if (uiGs->intro)
        {
            SDLW_IntroScreen(renderer, fonts);
            if (SDL_PollEvent(&event))
                handleEvents(uiGs, &event, WAITING);
        }
        else if (uiGs->core->gameOver)
        {
            playedBgMusic = false;
            if (!playedGameOverMusic)
            {
                Mix_PlayMusic(GetMusic(audio, "gameover"), 0);
                Mix_FadeOutMusic(3000);
                playedGameOverMusic = true;
            }
            SDLW_GameOverScreen(renderer, fonts, uiGs->core->score);
            if (SDL_PollEvent(&event))
                handleEvents(uiGs, &event, LOST);
        }
        else
        {
            playedGameOverMusic = false;
            if (!playedBgMusic)
            {
                Mix_FadeInMusic(GetMusic(audio, "bgm"), -1, 1500);
                playedBgMusic = true;
            }
            if (SDL_PollEvent(&event))
                handleEvents(uiGs, &event, PLAYING);

            handleCollision(uiGs->core);

            updateCars(uiGs->core);
            updateTrain(uiGs->core->grid);
            updateIce(uiGs->core);
            // Fin des possibles évents à "fire" ----

            // Calcul interactions (collisions)

            // maj état du jeu (états mobs, joueur, score)
            handleScore(uiGs->core);
            scrolling(uiGs->core);

            // maj rendu & rendu
            updateGameState(uiGs->core);
            SDLW_UpdateAndRender(uiGs, renderer, textures, fonts);
        }

        Uint64 frameEndTime = SDL_GetTicks64();
        Uint64 elapsedTime = frameEndTime - frameStartTime;

        if (elapsedTime < frameTime)
        {
            Uint64 sleepTime = frameTime - elapsedTime;
            SDL_Delay(sleepTime);
        }
    }

    SDLW_DestroyTextures(textures);
    SDLW_DestroyAudio(audio);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(fonts->large);
    TTF_CloseFont(fonts->medium);
    TTF_CloseFont(fonts->small);
    free(fonts);

    return 0;
}