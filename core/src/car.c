#include "car.h"
#include <stdlib.h>

void determineMaxesValues(int score, Occupation roadType, int *maxSize, int *maxSpeed, int *minSpeed, int *maxCars) {
    if (roadType == ROAD) {
        *maxSize = (score / 20 + 3 >= 6) ? 6 : score / 20 + 3;
        *minSpeed = (6 - score / 25 < 2) ? 2 : 6 - score / 25;
        *maxSpeed = *minSpeed + 3;
        *maxCars = (score >= 100) ? 4 : 2 + score / 20;
    } else if (roadType == WATER) {
        *maxSize = (6 - score / 20 < 3) ? 3 : 6 - score / 20;
        *minSpeed = (7 - score / 25 < 4) ? 4 : 7 - score / 25;
        *maxSpeed = *minSpeed + 1;
        *maxCars = (4 - score / 30 < 2) ? 2 : 4 - score / 30;
    }

    if (maxSpeed < minSpeed) maxSpeed = minSpeed;
}

void addCar(GameState *gs, int y, int forcedDirection, int availableSize, Occupation roadType)
{
    if (y < 0 || y >= gs->grid->height)
        return;
    if (gs->grid->cases[y][0] == SAFE || gs->grid->cases[y][0] == TREE)
        return;

    Car *baseCar = (Car *)malloc(sizeof(Car));
    if (!baseCar)
        return;

    int direction = forcedDirection == 0 ? (rand() % 2 ? 1 : -1) : forcedDirection;
    int startingX = (direction == 1) ? 0 : gs->grid->length - 1;

    int maxSize, maxSpeed, maxCars, minSpeed;
    
    determineMaxesValues(gs->score, roadType, &maxSize, &maxSpeed, &minSpeed, &maxCars);

    int size = 1 + rand() % (maxSize < availableSize ? maxSize : availableSize);
    int speed = minSpeed + rand() % (maxSpeed - minSpeed + 1);
    int desiredCars = 1 + rand() % maxCars;
    int loopTime = gs->grid->length * speed;

    *baseCar = (Car){.x = startingX, .y = y, .size = size, .direction = direction, .speed = speed, .accumulator = 0, .type = roadType};

    // roadType : When we have ROAD, then we know for sure that we have cars. When we have WATER, we know we have logs.
    gs->grid->cases[y][startingX] = roadType == WATER ? LOG : direction == 1 ? CAR_RIGHT
                                                                             : CAR_LEFT;
    addLastCar(gs->cars, baseCar);

    availableSize -= size;
    desiredCars--;

    int cumulativeCooldown = 0;
    int lastSize = size;

    while (desiredCars > 0 && availableSize > 0)
    {
        int nextSize = 1 + rand() % (maxSize < availableSize ? maxSize : availableSize);
        int spacing = 5 + rand() % 3;

        int safeCooldown = (lastSize + spacing) * speed;
        cumulativeCooldown += safeCooldown;

        if (cumulativeCooldown >= loopTime)
            break;

        Car *nextCar = (Car *)malloc(sizeof(Car));
        if (!nextCar)
            return;

        *nextCar = (Car){.x = startingX, .y = y, .size = nextSize, .direction = direction, .speed = speed, .accumulator = 0};

        Effect *e = (Effect *)malloc(sizeof(Effect));
        if (!e)
        {
            free(nextCar);
            return;
        }

        e->function = &addLastCar;
        e->car = nextCar;
        e->cooldown = cumulativeCooldown;

        addLastEffect(gs->effects, e);

        lastSize = nextSize;
        availableSize -= nextSize;
        desiredCars--;
    }
}

void updateCars(GameState *gs)
{
    CarElement *cursor = gs->cars->head;
    while (cursor != NULL)
    {
        Car *c = cursor->car;
        CarElement *next = cursor->next;

        // The car's accumulator will climb with each frame incrementing it. Once it reaches speed, it will reset and make the car moves forward in the same frame.
        // e.g.: A car with a speed of 10 will need 10 frames to move forward, so in 60 FPS he will move forward every 1/6 second, making him moves forward 6 times per second. 
        // Formula is : FPS/Speed.

        if (c->accumulator == c->speed)
        {
            for (int i = 0; i < c->size; i++) // Clear the road.
                if (c->y >= 0 && c->y < gs->grid->height && c->x + i * c->direction >= 0 && c->x + i * c->direction < gs->grid->length)
                    gs->grid->cases[c->y][c->x + i * c->direction] = c->type;

            int newX = c->x + c->direction;
            c->x = (newX < 0) ? gs->grid->length - 1 : (newX >= gs->grid->length ? 0 : newX);
            c->accumulator = 0;

            if (c->type == WATER && gs->player->y == c->y) // If player is on water, game should stop, unless he is on a log.
                gs->player->x += c->direction;             // On a log, the player moves too.

            for (int i = 0; i < c->size; i++) // Put the car at his new position
                if (c->y >= 0 && c->y < gs->grid->height && c->x + i * c->direction >= 0 && c->x + i * c->direction < gs->grid->length)
                    gs->grid->cases[c->y][c->x + i * c->direction] = c->type == WATER ? LOG : c->direction == 1 ? CAR_RIGHT
                                                                                                                : CAR_LEFT;
        }
        else
        {
            c->accumulator++;
        }

        cursor = next;
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
