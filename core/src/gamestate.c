#include "gamestate.h"
#define AI

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
    gs->highestLineReached = gs->player->y;

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
        gs->highestLineReached = 2;
        if (gs->grid->rowManagers[0]->type != SAFE)
            removeRowCar(gs->cars, 0);

        for (int i = 0; i < gs->grid->height - 1; i++)
        {
            gs->grid->cases[i] = gs->grid->cases[i + 1];
            gs->grid->rowManagers[i] = gs->grid->rowManagers[i + 1];
        }

        #ifdef AI 
            Occupation roadType = ROAD;
        #endif

        #ifndef AI 
            int r = rand() % 100;                                    // 50%
            Occupation roadType = (r < 50) ? ROAD : (r < 90) ? WATER // 40%
                                                            : RAIL; // 10%
        

        

        if ((rand() % 12 == 0) || (rand() % 5 == 0 && gs->grid->rowManagers[gs->grid->height - 2]->type == ICE))
            if (gs->grid->rowManagers[gs->grid->height - 6]->type != ICE &&
                gs->grid->rowManagers[gs->grid->height - 2]->type != SAFE)
                roadType = ICE;
        #endif
        gs->grid->cases[gs->grid->height - 1] = createRow(gs->grid->length, roadType);

        int newRowDirection = 1;
        int newRowSpeed = 30;

        if (roadType == WATER || roadType == ROAD)
        {
            newRowDirection = rand() % 2 ? 1 : -1;
            if (gs->grid->rowManagers[gs->grid->height - 2]->type == WATER)
                newRowDirection = gs->grid->rowManagers[gs->grid->height - 2]->direction * -1;

            int maxSpeed = 30;
            int minSpeed = 8;

            int scoreEffect = gs->score / 50;

            newRowSpeed = maxSpeed - scoreEffect;

            if (newRowSpeed < minSpeed)
                newRowSpeed = minSpeed;
            if (newRowSpeed > maxSpeed)
                newRowSpeed = maxSpeed;

            int variation = rand() % 5;
            newRowSpeed += variation;

            int prevSpeed = gs->grid->rowManagers[gs->grid->height - 1]->speed;
            if (prevSpeed <= newRowSpeed + 5 && prevSpeed >= newRowSpeed - 5)
                newRowSpeed = prevSpeed + 6;

            if (newRowSpeed > maxSpeed)
                newRowSpeed = maxSpeed;
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

TrainState updateTrain(Grid *grid)
{
    for (int y = 0; y < grid->height; y++)
    {
        RowManager *rm = grid->rowManagers[y];

        if (rm->type == RAIL || rm->type == WARNING || rm->type == TRAIN)
            rm->cooldown++;

        switch (rm->type)
        {
        case RAIL:
            if (rm->cooldown >= rm->speed)
            {
                rm->type = WARNING;
                rm->cooldown = 0;
                rm->speed = 2 * 60;

                for (int x = 0; x < grid->length; x++)
                    if (x % 2 == 0)
                        grid->cases[y][x] = WARNING;

                return HORN;
            }
            break;

        case WARNING:
            if (rm->cooldown >= rm->speed)
            {
                rm->type = TRAIN;
                rm->cooldown = 0;
                rm->speed = 8 * 60;
                applyOccupationToRow(grid->cases[y], grid->length, TRAIN);
                return PASSING;
            }
            break;

        case TRAIN:
            if (rm->cooldown >= rm->speed)
            {
                rm->type = RAIL;
                rm->cooldown = 0;

                rm->speed = 16 * 60 + rand() % (12 * 60 + 1);

                applyOccupationToRow(grid->cases[y], grid->length, RAIL);
                return AWAY;
            }
            break;

        default:
            break;
        }
    }
    return AWAY;
}

void updateIce(GameState *gs)
{
    Player *p = gs->player;
    int y = p->y;

    if (y < 0)
        return;

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

CollisionState handleCollision(GameState *gs)
{
    // Previous : We check every car and see if a car is colliding with the player.
    // New : Check directly on the map if the user is stepping on a CAR cell.
    // Pros : Faster. Less Code. Easier to read. Better for future implementation, like here with LOG and Water.
    // We don't even need to mind safe zone and logs.
    // Colliding with the void (out of the viewable map)
    if (gs->player->x < gs->carMaxSize || gs->player->x > gs->grid->length - gs->carMaxSize + 1 || gs->player->y < 0)
    {
        gs->gameOver = true;
        return OUT_OF_MAP;
    }

    Occupation playerOccupation = gs->grid->cases[gs->player->y][gs->player->x];
    // Colliding with an object.
    if (playerOccupation == CAR_LEFT || playerOccupation == CAR_RIGHT || playerOccupation == TRAIN)
    {
        gs->gameOver = true;
        return CRASHED;
    }
    else if (playerOccupation == WATER)
    {
        gs->gameOver = true;
        return SPLASHED;
    }
    return OUT_OF_MAP;
}

void updateGameState(GameState *gs)
{
    displayGrid(gs->grid, gs->score, gs->player->x, gs->player->y, gs->carMaxSize);
    // printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    // printCarQueue(gs->cars); // Pour faire de la debug
    // printEffectQueue(gs->effects);
    // printf("%d, %d", gs->player->x, gs->player->y);
}

void handleScore(GameState *gs)
{
    if (gs->player->y > gs->highestLineReached)
    {
        gs->score++;
        gs->highestLineReached = gs->player->y;
    }
}