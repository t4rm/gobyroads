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
// AI features, directly from core
#include "agent_ai.h"
#include "a_star.h"
// Standard
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Fonts *fonts = (TTF_Fonts *)malloc(sizeof(TTF_Fonts));
    // For AI :
    int pathLength = 0;
    int cmpt = 0;
    Node **path = NULL;
    // ----

    if (SDLW_Initialize(&window, &renderer, &fonts, WIDTH, HEIGHT) != 0)
        exit(EXIT_FAILURE);

    TextureCollection *textures = SDLW_InitTextures(renderer);
    AudioCollection *audio = SDLW_InitAudio();

    UIGameState *uiGs = initUIGameState(ROWS, COLS);
    // Plutôt utiliser les vraies dimensions du coup.
    // Et appliquer un coefficient de réduction pour le comportement du jeu en CLI ?
    // Comment gérer le fait qu'une coordonnée est representé en 1 pixel ici.
    SDL_Event event;
    const int FPS = 60;
    const int frameTime = 1000 / FPS;
    bool playedGameOverMusic = false;
    bool playedBgMusic = false;

    while (uiGs->running)
    {
        Uint64 frameStartTime = SDL_GetTicks64();
        if (uiGs->muteSound)
            Mix_VolumeMusic(0);
        else
            Mix_VolumeMusic(MIX_MAX_VOLUME / 4);

        if (uiGs->menu)
        {
            SDLW_MenuScreen(renderer, fonts, uiGs->menuHandler);
            if (SDL_PollEvent(&event))
                handleEvents(uiGs, &event, MENU);
        }
        else if (uiGs->intro)
        {
            SDLW_IntroScreen(renderer, fonts);
            if (SDL_PollEvent(&event))
                handleEvents(uiGs, &event, WAITING);
        }
        else if (uiGs->core->gameOver)
        {
            SDL_ShowWindow(window);
            SDLW_Mix_HaltAllChannelExcept(SFX_CHANNEL);

            if (SDL_PollEvent(&event))
                handleEvents(uiGs, &event, LOST);

            playedBgMusic = false;
            if (!playedGameOverMusic)
            {
                Mix_PlayMusic(GetMusic(audio, "gameover"), 0);
                Mix_FadeOutMusic(3000);
                playedGameOverMusic = true;
            }
            SDLW_GameOverScreen(renderer, fonts, uiGs->core->score);
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

            if (uiGs->core->player->afk >= FPS * 6)
                uiGs->core->gameOver = true;

            updateCars(uiGs->core);
            // AI :
            if (uiGs->menuHandler->selectedOptions[OPTION_IA - 1])
            {
                uiGs->core->player->mouvementCooldown = uiGs->core->player->mouvementCooldown <= 1 ? 0 : uiGs->core->player->mouvementCooldown - 1;
                uiGs->core->player->afk++;

                if (cmpt >= pathLength)
                {
                    path = getPathAI(uiGs->core, &pathLength);
                    cmpt = 0;
                }
                if (cmpt < pathLength)
                {
                    if (uiGs->core->player->mouvementCooldown == 0)
                    {
                        playerMoveAi(uiGs->core, path[cmpt]);
                        cmpt++;
                    }
                }
            }
            // -------------------------------
            SDLW_UpdateTrain(uiGs->core->grid, audio);
            updateIce(uiGs->core);
            // Fin des possibles évents à "fire" ----
            // Calcul interactions (collisions)
            SDLW_HandleCollision(uiGs->core, audio);
            // maj état du jeu (états mobs, joueur, score)
            handleScore(uiGs->core);
            scrolling(uiGs->core, uiGs->menuHandler->selectedOptions[OPTION_IA - 1]);

            // maj rendu & rendu
            if (uiGs->menuHandler->selectedOptions[OPTION_CORE - 1]) // Core checked
            {
                updateGameState(uiGs->core); // Display the grid
                if (!uiGs->menuHandler->selectedOptions[OPTION_IA - 1])
                    playerMove(uiGs->core); // Listen to terminal keys only if not in AI mode

                if (!uiGs->menuHandler->selectedOptions[OPTION_UI - 1]) // No UI
                    SDL_HideWindow(window);
            }

            if (uiGs->menuHandler->selectedOptions[OPTION_UI - 1]) // UI checked
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
    TTF_CloseFont(fonts->monospaced);
    free(fonts);

    return 0;
}