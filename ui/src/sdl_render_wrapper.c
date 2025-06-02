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

/* A wrapper function to Render a Text
 * x, y: the coordinates of the text
 * w, h: the size of the text
 * font: the font to use
 * renderer: a pointer to the renderer
 * text: the string to print
 * color: an enumeration of colors that are converted from ENUM to RGBA (hardcoded)
 */
void SDLW_RenderText(int x, int y, int w, int h, TTF_Font *font, SDL_Renderer *renderer, char *text, SDLW_COLORS color)
{
    SDL_Color textColor = {255, 255, 255, 255};
    if (color == GREEN)
        textColor = (SDL_Color){62, 167, 66, 255};

    int textWidth, textHeight;
    if (TTF_SizeText(font, text, &textWidth, &textHeight))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error getting text size: %s", SDL_GetError());
        return;
    }

    int centerX = x + (w - textWidth) / 2;
    int centerY = y + (h - textHeight) / 2;

    SDL_Surface *surfaceText = TTF_RenderText_Solid(font, text, textColor);
    if (!surfaceText)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error creating text surface: %s", SDL_GetError());
        return;
    }

    SDL_Texture *textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
    if (!textureText)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error creating text texture: %s", SDL_GetError());
        SDL_FreeSurface(surfaceText);
        return;
    }

    SDL_Rect rectangleText = {centerX, centerY, textWidth, textHeight};

    SDL_RenderCopy(renderer, textureText, NULL, &rectangleText);
    SDL_DestroyTexture(textureText);
    SDL_FreeSurface(surfaceText);
}

/* A wrapper function for the RenderCopy of SDL
 * r: a pointer to the renderer
 * t: a pointer to the texture
 * x, y: the coordinates indicating where to print the texture
 * xOffset, yOffset: the offsets to get the right sprite from the spritesheet
 * flip: the flip to apply on the texture (used to transform a east looking car to a west looking car)
 * yDepth: edit the y coordinate, creating a fake depth in our isometric view
 * optionalSpriteSize: used when the spritesheet is not in perfect square but rectangles, this is the length of the height of a sprite in a spritesheet
 */
void SDLW_RenderCopy(SDL_Renderer *r, SDL_Texture *t, int x, int y,
                     int xOffset, int yOffset, SDL_RendererFlip flip,
                     int yDepth, int spriteSize, int xDepth, int optionalSpriteSize)
{
    optionalSpriteSize = optionalSpriteSize == 0 ? spriteSize : optionalSpriteSize;

    SDL_Rect spriteRect = {
        xOffset * spriteSize,
        yOffset * optionalSpriteSize,
        spriteSize,
        optionalSpriteSize};

    int logicalX = (x - CAR_MAX_SIZE - 1) * CELL_SIZE + xDepth;
    int logicalY = flipY(y) * CELL_SIZE - yDepth;

    int offsetX = (spriteSize - CELL_SIZE) / 2;
    int offsetY = (optionalSpriteSize - CELL_SIZE) / 2;

    SDL_Rect destRect = {
        logicalX - offsetX,
        logicalY - offsetY,
        spriteSize,
        optionalSpriteSize};

    if (SDL_RenderCopyEx(r, t, &spriteRect, &destRect, 0.0, NULL, flip))
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
}
