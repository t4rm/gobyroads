#include "sdl_render_wrapper.h"

// void SDLW_RenderText(int x, int y, int w, int h, TTF_Font *font, SDL_Renderer *renderer, char *text)
// {
//     SDL_Rect rectangleTitle = {x, y, w, h};
//     SDL_Color textColor = {255, 255, 255, 255};
//     SDL_Surface *surfaceText = TTF_RenderText_Solid(font, text, textColor);
//     SDL_Texture *textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
//     SDL_FreeSurface(surfaceText);
//     SDL_RenderCopy(renderer, textureText, NULL, &rectangleTitle);
// }

void SDLW_RenderText(int x, int y, int w, int h, TTF_Font *font, SDL_Renderer *renderer, char *text)
{
    SDL_Color textColor = {255, 255, 255, 255};

    int textWidth, textHeight;
    if (TTF_SizeText(font, text, &textWidth, &textHeight)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error getting text size: %s", SDL_GetError());
        return;
    }

    int centerX = x + (w - textWidth) / 2; 
    int centerY = y + (h - textHeight) / 2;

    SDL_Surface *surfaceText = TTF_RenderText_Solid(font, text, textColor);
    if (!surfaceText) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error creating text surface: %s", SDL_GetError());
        return;
    }

    SDL_Texture *textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
    if (!textureText) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error creating text texture: %s", SDL_GetError());
        SDL_FreeSurface(surfaceText);
        return;
    }

    SDL_Rect rectangleText = {centerX, centerY, textWidth, textHeight};

    SDL_RenderCopy(renderer, textureText, NULL, &rectangleText);
    SDL_DestroyTexture(textureText);
    SDL_FreeSurface(surfaceText);
}


// This work fine for dynamic sprite like Player, but this is "OVERKILL" for Static Sprites like Grass.
// Grass don't have offsets, but we could ! They would become animated (simulating the wind for example)
// Same for waves in Rivers
void SDLW_RenderCopy(SDL_Renderer *r, SDL_Texture *t, int x, int y,
                     int xOffset, int yOffset, SDL_RendererFlip flip,
                     int yDepth, int spriteSize, int xDepth)
{
    SDL_Rect spriteRect = {
        xOffset * spriteSize,
        yOffset * spriteSize,
        spriteSize,
        spriteSize};

    int logicalX = (x - CAR_MAX_SIZE - 1) * CELL_SIZE + xDepth;
    int logicalY = flipY(y) * CELL_SIZE - yDepth;

    int offsetX = (spriteSize - CELL_SIZE) / 2;
    int offsetY = (spriteSize - CELL_SIZE) / 2;

    SDL_Rect destRect = {
        logicalX - offsetX,
        logicalY - offsetY,
        spriteSize,
        spriteSize};

    if (SDL_RenderCopyEx(r, t, &spriteRect, &destRect, 0.0, NULL, flip))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
    }
}
