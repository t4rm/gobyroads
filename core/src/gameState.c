#include "gamestate.h"

GameState *initGameState(int h, int l)
{
    GameState *gs = (GameState *)malloc(sizeof(GameState));
    gs->carMaxSize = 6;
    gs->cars = createCarQueue();
    gs->grid = createGrid(h, l, gs->carMaxSize);
    gs->player = (Player *)malloc(sizeof(Player));
    gs->player->x = (l + 2 * gs->carMaxSize + 1) / 2;
    gs->player->y = 0;
    gs->player->mouvementCooldown = 0;
    gs->player->afk = 0;
    gs->score = 0;
    gs->backwardMovements = 0;
    gs->gameOver = false;
    gs->nextSafeZone = 3;

    for (int i = 0; i < gs->grid->height; i++)
    {
        if (gs->nextSafeZone == i)
        {
            applyOccupationToRow(gs->grid->cases[gs->nextSafeZone], l + 2 * gs->carMaxSize, SAFE);
            gs->grid->rowManagers[gs->nextSafeZone]->type = SAFE;
            gs->nextSafeZone += 4;
        }
        else
        {
            switch (gs->grid->rowManagers[i]->type)
            {
            case ROAD:
            case WATER:
                addCar(gs, i, gs->grid->rowManagers[i], gs->grid->length);
                break;
            default:
                break;
            }
        }
    }

    gs->grid->cases[gs->player->y][gs->player->x] = SAFE; // On spawn sur une safe zone, pas un arbre.

    printf("\e[1;1H\e[2J"); // Nettoyage de l'écran
    printf("\e[?25l");      // Cacher le curseur

    return gs;
}

void destroyGameState(GameState *gs)
{
    destroyCarQueue(gs->cars);
    destroyGrid(gs->grid);
    free(gs->player);
    free(gs);
}

void scrolling(GameState *gs)
{

    if (gs->player->y == 3)
    {
        gs->score++;
        if (gs->grid->rowManagers[0]->type != SAFE)
            removeRowCar(gs->cars, 0);

        for (int i = 0; i < gs->grid->height - 1; i++)
        {
            gs->grid->cases[i] = gs->grid->cases[i + 1];
            gs->grid->rowManagers[i] = gs->grid->rowManagers[i + 1];
        }

        int r = rand() % 10;
        Occupation roadType = (r < 5) ? ROAD : (r < 9) ? WATER
                                                       : RAIL;

        // makes ice rare, 1 to 5 row long "biomes"
        if ((rand() % 8 == 0) || (rand() % 3 == 0 && gs->grid->rowManagers[gs->grid->height - 2]->type == ICE))
            if (gs->grid->rowManagers[gs->grid->height - 6]->type != ICE && gs->grid->rowManagers[gs->grid->height - 2]->type != SAFE)
                roadType = ICE;

        gs->grid->cases[gs->grid->height - 1] = createRow(gs->grid->length, roadType);

        int newRowDirection = 1;
        int newRowSpeed = 30;
        if (roadType == WATER || roadType == ROAD)
        {
            newRowDirection = rand() % 2 ? 1 : -1;
            if (gs->grid->rowManagers[gs->grid->height - 2]->type == WATER)
                newRowDirection = gs->grid->rowManagers[gs->grid->height - 2]->direction * -1;

            // scaling de la vitesse en fonction du score
            int maxFrameCooldown = 30;
            newRowSpeed = (maxFrameCooldown - 14 - gs->score / 50) < 9 ? 8 : maxFrameCooldown - (rand() % 14) - (gs->score / 50);

            // verifie que 2 lignes qui se suivent n'ont pas de vitesses trop proches
            if (gs->grid->rowManagers[gs->grid->height - 1]->speed <= newRowSpeed + 5 &&
                gs->grid->rowManagers[gs->grid->height - 1]->speed >= newRowSpeed - 5)
                newRowSpeed = gs->grid->rowManagers[gs->grid->height - 1]->speed + 10;
        }

        if (roadType == RAIL)
            newRowSpeed = 400;

        gs->grid->rowManagers[gs->grid->height - 1] = createRowManager(newRowDirection, newRowSpeed, roadType);
        gs->player->y--;

        decrementCarsOnY(gs);

        if (gs->grid->rowManagers[gs->grid->height - 2]->type != ICE)
            gs->nextSafeZone--;

        if (gs->nextSafeZone == 0)
        {
            applyOccupationToRow(gs->grid->cases[gs->grid->height - 1], gs->grid->length, SAFE);
            gs->grid->rowManagers[gs->grid->height - 1]->type = SAFE;
            gs->nextSafeZone = 3 + rand() % (1 + (gs->score / 10));
        }
        else if (roadType == WATER || roadType == ROAD)
            addCar(gs, gs->grid->height - 1, gs->grid->rowManagers[gs->grid->height - 1], gs->grid->length);
    }
}

void updateTrain(Grid *grid)
{
    for (int y = 0; y < grid->height; y++)
    {
        RowManager *rm = grid->rowManagers[y];

        if (rm->type == RAIL || rm->type == WARNING || rm->type == TRAIN)
            rm->cooldown++;

        if (rm->type == RAIL && rm->cooldown >= rm->speed)
        {
            rm->type = WARNING;
            rm->cooldown = 0;
            rm->speed /= 4;

            for (int x = 0; x < grid->length; x++)
                if (x % 2 == 0)
                    grid->cases[y][x] = WARNING;
        }
        else if (rm->type == WARNING && rm->cooldown >= rm->speed)
        {
            rm->type = TRAIN;
            rm->cooldown = 0;
            rm->speed /= 2;
            applyOccupationToRow(grid->cases[y], grid->length, TRAIN);
        }
        else if (rm->type == TRAIN && rm->cooldown >= rm->speed)
        {
            rm->type = RAIL;
            rm->cooldown = 0;
            rm->speed *= 8;
            applyOccupationToRow(grid->cases[y], grid->length, RAIL);
        }
    }
}

void updateIce(GameState *gs)
{
    Player *p = gs->player;
    int y = p->y;

    if (gs->grid->rowManagers[y]->type == ICE && --p->slidingCooldown <= 0)
    {
        bool moved = false;
        if (p->lastMoove == 1)
            playerMoveKey('z', gs, &moved); // haut
        else if (p->lastMoove == -1)
            playerMoveKey('s', gs, &moved); // bas
        else if (p->lastMoove == 2)
            playerMoveKey('q', gs, &moved); // gauche
        else if (p->lastMoove == -2)
            playerMoveKey('d', gs, &moved); // droite

        p->slidingCooldown = 36;
    }
}

void handleCollision(GameState *gs)
{
    // Previous : We check every car and see if a car is colliding with the player.
    // New : Check directly on the map if the user is stepping on a CAR cell.
    // Pros : Faster. Less Code. Easier to read. Better for future implementation, like here with LOG and Water.
    // We don't even need to mind safe zone and logs.
    // Colliding with the void (out of the viewable map)
    if (gs->player->x < gs->carMaxSize || gs->player->x > gs->grid->length - gs->carMaxSize + 1 || gs->player->y < 0)
    {
        gs->gameOver = true;
        return;
    }

    Occupation playerOccupation = gs->grid->cases[gs->player->y][gs->player->x];
    // Colliding with an object.
    if (playerOccupation == CAR_LEFT || playerOccupation == CAR_RIGHT || playerOccupation == WATER || playerOccupation == TRAIN)
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
