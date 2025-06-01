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

    SDLW_RenderText(WIDTH / 4, startY, WIDTH / 2, 80, fonts->large, renderer, s, WHITE);
    SDLW_RenderText(WIDTH / 6, startY + textHeight + verticalMargin, WIDTH - 4 * CELL_SIZE, 72, fonts->medium, renderer, s2, WHITE);
    SDLW_RenderText(WIDTH / 6, startY + 2 * (textHeight + verticalMargin), WIDTH - 4 * CELL_SIZE, 72, fonts->medium, renderer, s3, WHITE);

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
    char s7[22] = "and M to toggle Music";

    int verticalMargin = 20;
    int totalHeight = 4 * 72 + 3 * verticalMargin;
    int startY = (HEIGHT - totalHeight) / 3;
    SDLW_RenderText(WIDTH / 4, startY, WIDTH / 2, 80, fonts->large, renderer, s, WHITE);
    SDLW_RenderText(WIDTH / 6, startY + 80 + verticalMargin, WIDTH - 4 * CELL_SIZE, 72, fonts->small, renderer, s2, WHITE);
    SDLW_RenderText(WIDTH / 6, startY + 80 + 2 * verticalMargin + 72, WIDTH - 4 * CELL_SIZE, 72, fonts->medium, renderer, s3, WHITE);
    SDLW_RenderText(WIDTH / 6, startY + 80 + 3 * verticalMargin + 72 + 24, WIDTH - 4 * CELL_SIZE, 72, fonts->medium, renderer, s4, WHITE);
    SDLW_RenderText(WIDTH / 6, startY + 80 + 5 * verticalMargin + 2 * 72, WIDTH - 4 * CELL_SIZE, 72, fonts->large, renderer, s5, WHITE);
    SDLW_RenderText(WIDTH / 6, startY + 80 + 6 * verticalMargin + 2 * 72 + 24, WIDTH - 4 * CELL_SIZE, 72, fonts->small, renderer, s6, WHITE);
    SDLW_RenderText(WIDTH / 6, startY + 80 + 8 * verticalMargin + 2 * 72 + 24, WIDTH - 4 * CELL_SIZE, 72, fonts->small, renderer, s7, WHITE);

    SDL_RenderPresent(renderer);
}

void SDLW_MenuScreen(SDL_Renderer *renderer, TTF_Fonts *fonts, MenuHandler *menuHandler)
{
    SDL_Color const BACKGROUND_COLOR = {.r = 0, .g = 0, .b = 0, .a = SDL_ALPHA_OPAQUE};
    if (SDL_SetRenderDrawColor(renderer,
                               BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a))
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in set render draw color: %s", SDL_GetError());

    if (SDL_RenderClear(renderer))
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render clear: %s", SDL_GetError());

    char s[11] = "Goby Roads";
    char s2[9] = "[ ] CORE";
    char s3[9] = "[ ] UI  ";
    char s4[9] = "[ ] IA  ";

    if (menuHandler->selectedOptions[0] == 1)
        s2[1] = '#';
    if (menuHandler->selectedOptions[1] == 1)
        s3[1] = '#';
    if (menuHandler->selectedOptions[2] == 1)
        s4[1] = '#';

    char s5[28] = "Use UP/DOWN Arrow to Select";
    char s6[23] = "Press Enter to confirm";
    char s7[21] = "Press Space to start";

    int verticalMargin = 20;
    int totalHeight = 4 * 72 + 3 * verticalMargin;
    int startY = (HEIGHT - totalHeight) / 3;
    SDLW_RenderText(WIDTH / 4, startY, WIDTH / 2, 80, fonts->large, renderer, s, WHITE);

    SDLW_RenderText(WIDTH / 6, startY + 80 + verticalMargin, WIDTH - 4 * CELL_SIZE, 72, fonts->monospaced, renderer, s2, menuHandler->highligtedOption == OPTION_CORE ? GREEN : WHITE);
    SDLW_RenderText(WIDTH / 6, startY + 80 + 4 * verticalMargin, WIDTH - 4 * CELL_SIZE, 72, fonts->monospaced, renderer, s3, menuHandler->highligtedOption == OPTION_UI ? GREEN : WHITE);
    SDLW_RenderText(WIDTH / 6, startY + 80 + 7 * verticalMargin, WIDTH - 4 * CELL_SIZE, 72, fonts->monospaced, renderer, s4, menuHandler->highligtedOption == OPTION_IA ? GREEN : WHITE);

    SDLW_RenderText(WIDTH / 6, startY + 80 + 6 * verticalMargin + 72 + 24, WIDTH - 4 * CELL_SIZE, 72, fonts->small, renderer, s5, WHITE);
    SDLW_RenderText(WIDTH / 6, startY + 80 + 7 * verticalMargin + 2 * 72, WIDTH - 4 * CELL_SIZE, 72, fonts->medium, renderer, s6, WHITE);
    SDLW_RenderText(WIDTH / 6, startY + 80 + 10 * verticalMargin + 2 * 72, WIDTH - 4 * CELL_SIZE, 72, fonts->medium, renderer, s7, WHITE);

    SDL_RenderPresent(renderer);
}
