#include "car.h"

// void determineMaxesValues(int score, Occupation roadType, int *maxSize, int *maxSpeed, int *minSpeed, int *maxCars)
// {
//     if (roadType == ROAD)
//     {
//         *maxSize = (score / 20 + 3 >= 5) ? 5 : score / 20 + 3;
//         *minSpeed = (5 - score / 25 < 2) ? 2 : 5 - score / 25;
//         *maxSpeed = *minSpeed + 3;
//         *maxCars = (score >= 100) ? 4 : 2 + score / 20;
//     }
//     else if (roadType == WATER)
//     {
//         *maxSize = (5 - score / 20 < 3) ? 3 : 5 - score / 20;
//         *minSpeed = (7 - score / 25 < 4) ? 4 : 7 - score / 25;
//         *maxSpeed = *minSpeed + 1;
//         *maxCars = (4 - score / 30 < 2) ? 2 : 4 - score / 30;
//     }

//     if (maxSpeed < minSpeed)
//         maxSpeed = minSpeed;
// }

void addCar(GameState *gs, int y, RowManager *rowManager, int availableSize)
{
    if (y < 0 || y >= gs->grid->height)
        return;
    if (gs->grid->rowManagers[y]->type == SAFE)
        return;

    int direction = rowManager->direction;
    int startingX = 0;

    int minSize = rowManager->type == WATER ? 2 : 1;
    int maxSize = rowManager->type == WATER ? ((5 - gs->score / 20 < 3) ? 3 : 5 - gs->score / 20) : ((gs->score / 20 + 3 >= 5) ? 5 : gs->score / 20 + 3);
    int minSpacing = 3;
    int maxAddedSpacing = rowManager->type == WATER ? 5 : 12;
    int lastSize = 0;

    while (availableSize > minSize + minSpacing)
    {
        int nextSize = minSize + rand() % maxSize;
        int spacing = minSpacing + rand() % maxAddedSpacing;

        while (spacing + nextSize > availableSize)
            if (spacing > minSpacing)
                spacing -= 1;
            else
                nextSize -= 1;

        Car *nextCar = (Car *)malloc(sizeof(Car));
        if (!nextCar)
            return;

        *nextCar = (Car){.x = startingX, .y = y, .size = nextSize, .direction = direction};

        nextCar->x = startingX + lastSize + spacing;
        if (nextCar->x < 0)
            nextCar->x += gs->grid->length - 1;
        if (nextCar->x >= gs->grid->length)
            nextCar->x -= gs->grid->length - 1;
        addLastCar(gs->cars, nextCar);
        startingX += lastSize + spacing;
        lastSize = nextSize;
        availableSize -= nextSize + spacing;
    }
}

void updateCars(GameState *gs)
{
    CarElement *cursor = gs->cars->head;
    while (cursor != NULL)
    {
        Car *c = cursor->car;
        CarElement *next = cursor->next;

        if (gs->grid->rowManagers[c->y]->cooldown == gs->grid->rowManagers[c->y]->speed)
        {
            for (int i = 0; i < c->size; i++) // Clear the road.
                if (c->y >= 0 && c->y < gs->grid->height && c->x + i * c->direction >= 0 && c->x + i * c->direction < gs->grid->length)
                    gs->grid->cases[c->y][c->x + i * c->direction] = gs->grid->rowManagers[c->y]->type;

            if (gs->grid->rowManagers[c->y]->type == WATER && gs->player->y == c->y) // If player is on water, game should stop, unless he is on a log.
            {
                int endingX = c->x + c->size * c->direction + c->direction * -1;
                int startingX = c->x;
                if (startingX > endingX)
                {
                    int t = startingX;
                    startingX = endingX;
                    endingX = t;
                }

                if (gs->player->x >= startingX && gs->player->x <= endingX)
                    gs->player->x += c->direction; // On a log, the player moves too.
            }

            int newX = c->x + c->direction;
            c->x = (newX < 0) ? gs->grid->length - 1 : (newX >= gs->grid->length ? 0 : newX);

            for (int i = 0; i < c->size; i++) // Put the car at his new position
                if (c->y >= 0 && c->y < gs->grid->height && c->x + i * c->direction >= 0 && c->x + i * c->direction < gs->grid->length)
                    gs->grid->cases[c->y][c->x + i * c->direction] =
                        gs->grid->rowManagers[c->y]->type == WATER ? LOG : c->direction == 1 ? CAR_RIGHT
                                                                                             : CAR_LEFT;
        }
        cursor = next;
    }

    for (int i = 0; i <= gs->grid->height - 1; i++)
    {
        if (gs->grid->rowManagers[i]->cooldown == gs->grid->rowManagers[i]->speed)
            gs->grid->rowManagers[i]->cooldown = 0;
        else
            gs->grid->rowManagers[i]->cooldown += 1;
    }
}

void decrementCarsOnY(GameState *gs)
{
    CarElement *cursor = gs->cars->head;
    while (cursor != NULL)
    {
        if (cursor->car)
            cursor->car->y--;
        cursor = cursor->next;
    }
}
