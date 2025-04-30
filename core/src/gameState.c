#include "gamestate.h"

GameState *initGameState(int h, int l)
{
    GameState *gs = (GameState *)malloc(sizeof(GameState));
    gs->carMaxSize = 6;
    gs->cars = createCarQueue();
    gs->effects = createEffectQueue();
    gs->grid = createGrid(h, l, gs->carMaxSize);
    gs->player = (Player *)malloc(sizeof(Player));
    gs->player->x = (l + 2 * gs->carMaxSize) / 2;
    gs->player->y = 0;
    gs->player->mouvementCooldown = 0;
    gs->player->afk = 0;
    gs->score = 0;
    gs->backwardMovements = 0;
    gs->gameOver = false;
    gs->nextSafeZone = 3;

    while (gs->nextSafeZone < h)
    {
        applyOccupationToRow(gs->grid->cases[gs->nextSafeZone], l + 2 * gs->carMaxSize, SAFE);
        gs->nextSafeZone += 4 + (gs->score / 10);
    }

    gs->nextSafeZone = 3 + (gs->score / 10);

    for (int i = 0; i < gs->grid->height; i++)
        addCar(gs, i, 0, gs->grid->length/2);

    gs->grid->cases[gs->player->y][gs->player->x] = SAFE; // On spawn sur une safe zone, pas un arbre.

    printf("\e[1;1H\e[2J"); // Nettoyage de l'écran
    printf("\e[?25l"); // Cacher le curseur

    return gs;
}

void destroyGameState(GameState * gs) {
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
        if (gs->grid->cases[0][0] != SAFE && gs->grid->cases[0][0] != TREE) {
            removeRowCar(gs->cars, 0);
            removeRowEffect(gs->effects, 0);
        }

        for (int i = 0; i < gs->grid->height - 1; i++)
            gs->grid->cases[i] = gs->grid->cases[i + 1];

        gs->grid->cases[gs->grid->height - 1] = createRow(gs->grid->length, ROAD);

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
            addCar(gs, gs->grid->height - 1, 0, gs->grid->length/2);
    }
}

bool handleCollision(GameState *gs)
{
    if (gs->grid->cases[gs->player->y][0] == SAFE)
        return false;
    
    CarElement *cursor = gs->cars->head;
    while (cursor != NULL)
    {
        Car * c = cursor->car;
        if (c->y == gs->player->y) {
            int endingX = c->x + c->size * c->direction + c->direction * -1;
            int startingX = c->x;
            
            if (startingX > endingX) {
                int t = startingX;
                startingX = endingX;
                endingX = t;
            }
            
            if (gs->player->x >= startingX && gs->player->x <= endingX)
            {
                // printf("Collision sur l'intervalle [%d,%d] en Y = %d avec un joueur en %d,%d", startingX, endingX, c->y, gs->player->x, gs->player->y);
                gs->gameOver = true;
                return true;
            }
        }
        cursor = cursor->next;
    }
    return false;
}

void updateGameState(GameState *gs)
{
    displayGrid(gs->grid, gs->score, gs->player->x, gs->player->y, gs->carMaxSize);
    printf("\n");
    // printCarQueue(gs->cars); // Pour faire de la debug
    printEffectQueue(gs->effects);
    // printf("%d, %d", gs->player->x, gs->player->y);
}
