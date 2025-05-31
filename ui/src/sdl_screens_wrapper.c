#include "sdl_screens_wrapper.h"

void SDLW_GameOverScreen(SDL_Renderer *renderer, TTF_Fonts *fonts, int score)
{
    SDL_Color const BACKGROUND_COLOR = {.r = 0, .g = 0, .b = 0, .a = SDL_ALPHA_OPAQUE};
    if (SDL_SetRenderDrawColor(renderer,
                               BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a))
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in set render draw color: %s", SDL_GetError());

    if (SDL_RenderClear(renderer))
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render clear: %s", SDL_GetError());

    char s[12] = "Game Over !";
    char s2[16] = "Press F to Quit";
    char s3[16] = "Or R to Restart";

    int verticalMargin = 20;
    int textHeight = 72;
    int totalHeight = 3 * textHeight + 2 * verticalMargin;

    int startY = (HEIGHT - totalHeight) / 2;

    SDLW_RenderText(WIDTH / 4, startY, WIDTH / 2, 80, fonts->large, renderer, s);
    SDLW_RenderText(WIDTH / 6, startY + textHeight + verticalMargin, WIDTH - 4 * CELL_SIZE, 72, fonts->medium, renderer, s2);
    SDLW_RenderText(WIDTH / 6, startY + 2 * (textHeight + verticalMargin), WIDTH - 4 * CELL_SIZE, 72, fonts->medium, renderer, s3);

    SDL_RenderPresent(renderer);
}

void SDLW_IntroScreen(SDL_Renderer *renderer, TTF_Fonts *fonts)
{
    SDL_Color const BACKGROUND_COLOR = {.r = 0, .g = 0, .b = 0, .a = SDL_ALPHA_OPAQUE};
    if (SDL_SetRenderDrawColor(renderer,
                               BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a))
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in set render draw color: %s", SDL_GetError());

    if (SDL_RenderClear(renderer))
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render clear: %s", SDL_GetError());

    char s[11] = "Goby Roads";
    char s2[31] = "Help Goby returns to his World";
    char s3[20] = "Use ZQSD/ARROW keys";
    char s4[9] = "to moove";
    char s5[15] = "Press SPACEBAR";
    char s6[19] = "to start the game!";

    int verticalMargin = 20;
    int totalHeight = 4 * 72 + 3 * verticalMargin;
    int startY = (HEIGHT - totalHeight) / 3;
    SDLW_RenderText(WIDTH / 4, startY, WIDTH / 2, 80, fonts->large, renderer, s);
    SDLW_RenderText(WIDTH / 6, startY + 80 + verticalMargin, WIDTH - 4 * CELL_SIZE, 72, fonts->small, renderer, s2);
    SDLW_RenderText(WIDTH / 6, startY + 80 + 2 * verticalMargin + 72, WIDTH - 4 * CELL_SIZE, 72, fonts->medium, renderer, s3);
    SDLW_RenderText(WIDTH / 6, startY + 80 + 3 * verticalMargin + 72 + 24, WIDTH - 4 * CELL_SIZE, 72, fonts->medium, renderer, s4);
    SDLW_RenderText(WIDTH / 6, startY + 80 + 5 * verticalMargin + 2 * 72, WIDTH - 4 * CELL_SIZE, 72, fonts->large, renderer, s5);
    SDLW_RenderText(WIDTH / 6, startY + 80 + 6 * verticalMargin + 2 * 72 + 24, WIDTH - 4 * CELL_SIZE, 72, fonts->small, renderer, s6);

    SDL_RenderPresent(renderer);
}
