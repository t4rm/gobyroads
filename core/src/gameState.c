#include "gamestate.h"

GameState *initGameState(int h, int l, bool isWrapped)
{
    GameState *gs = (GameState *)malloc(sizeof(GameState));
    gs->carMaxSize = 6;
    gs->cars = createCarQueue();
    gs->effects = createEffectQueue();
    gs->grid = createGrid(h, l, gs->carMaxSize, isWrapped);
    gs->player = (Player *)malloc(sizeof(Player));
    gs->player->x = (l + 2 * gs->carMaxSize + 1) / 2;
    gs->player->y = 0;
    gs->player->mouvementCooldown = 0;
    gs->player->afk = 0;
    gs->score = 0;
    gs->backwardMovements = 0;
    gs->gameOver = false;
    gs->nextSafeZone = 3;
    gs->isWrapped = isWrapped;

    while (gs->nextSafeZone < h)
    {
        applyOccupationToRow(gs->grid->cases[gs->nextSafeZone], l + 2 * gs->carMaxSize, SAFE);
        gs->nextSafeZone += 4 + (gs->score / 10);
    }

    gs->nextSafeZone = 3 + (gs->score / 10);

    for (int i = 0; i < gs->grid->height; i++)
        addCar(gs, i, 0, gs->grid->length / 2, ROAD);

    gs->grid->cases[gs->player->y][gs->player->x] = SAFE; // On spawn sur une safe zone, pas un arbre.

    // if (!isWrapped)
    // {
    printf("\e[1;1H\e[2J"); // Nettoyage de l'écran
    printf("\e[?25l");      // Cacher le curseur
    // }

    return gs;
}

void destroyGameState(GameState *gs)
{
    destroyCarQueue(gs->cars);
    destroyEffectQueue(gs->effects);
    destroyGrid(gs->grid);
    free(gs->player);
    free(gs);
}

void scrolling(GameState *gs)
{
    if (gs->player->y == 3)
    {
        if (gs->grid->cases[0][0] != SAFE && gs->grid->cases[0][0] != TREE)
        {
            removeRowCar(gs->cars, 0);
            removeRowEffect(gs->effects, 0);
        }

        for (int i = 0; i < gs->grid->height - 1; i++)
            gs->grid->cases[i] = gs->grid->cases[i + 1];

        Occupation roadType = rand() % 2 ? ROAD : WATER;
        gs->grid->cases[gs->grid->height - 1] = createRow(gs->grid->length, roadType);

        gs->player->y--;

        decrementCarsOnY(gs);
        decrementEffectsOnY(gs);

        gs->nextSafeZone--;

        if (gs->nextSafeZone == 0)
        {
            applyOccupationToRow(gs->grid->cases[gs->grid->height - 1], gs->grid->length, SAFE);
            gs->nextSafeZone = 4 + (gs->score / 10);
        }
        else
            addCar(gs, gs->grid->height - 1, 0, gs->grid->length / 2, roadType);
    }
}

void handleCollision(GameState *gs)
{
    // Previous : We check every car and see if a car is colliding with the player.
    // New : Check directly on the map if the user is stepping on a CAR cell.
    // Pros : Faster. Less Code. Easier to read. Better for future implementation, like here with LOG and Water.
    // We don't even need to mind safe zone and logs.
    Occupation playerOccupation = gs->grid->cases[gs->player->y][gs->player->x];
    // Colliding with an object.
    if (playerOccupation == CAR_LEFT || playerOccupation == CAR_RIGHT || playerOccupation == WATER)
        gs->gameOver = true;
    // Colliding with the void (out of the viewable map)
    if (gs->player->x < gs->carMaxSize || gs->player->x > gs->grid->length - gs->carMaxSize + 1)
        gs->gameOver = true;
}

void updateGameState(GameState *gs)
{
    displayGrid(gs->grid, gs->score, gs->player->x, gs->player->y, gs->carMaxSize);
    // printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    // printCarQueue(gs->cars); // Pour faire de la debug
    // printEffectQueue(gs->effects);
    // printf("%d, %d", gs->player->x, gs->player->y);
}
