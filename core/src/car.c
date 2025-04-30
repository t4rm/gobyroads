#include "car.h"
#include <stdlib.h>

void addCar(GameState *gs, int y, int forcedDirection, int availableSize)
{
    if (y < 0 || y >= gs->grid->height) return;
    if (gs->grid->cases[y][0] == SAFE || gs->grid->cases[y][0] == TREE) return;

    Car *baseCar = (Car *)malloc(sizeof(Car));
    if (!baseCar) return;

    int direction = forcedDirection == 0 ? (rand() % 2 ? 1 : -1) : forcedDirection;
    int startingX = (direction == 1) ? 0 : gs->grid->length - 1;

    int maxSize = (gs->score / 20 + 3 >= 6) ? 6 : gs->score / 20 + 3;
    int size = 1 + rand() % (maxSize < availableSize ? maxSize : availableSize);

    int maxSpeed = (gs->score / 20 + 1 >= 5) ? 5 : gs->score / 20 + 1;
    int speed = 1 + rand() % maxSpeed;

    int maxCars = (gs->score >= 100) ? 4 : 2 + gs->score / 20;
    int desiredCars = 1 + rand() % maxCars;

    int loopTime = gs->grid->length * speed;

    *baseCar = (Car){.x = startingX, .y = y, .size = size, .direction = direction, .speed = speed, .accumulator = 0};
    gs->grid->cases[y][startingX] = direction == 1 ? CAR_RIGHT : CAR_LEFT;
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
        if (!nextCar) return;

        *nextCar = (Car){.x = startingX, .y = y, .size = nextSize, .direction = direction, .speed = speed, .accumulator = 0};

        Effect *e = (Effect *)malloc(sizeof(Effect));
        if (!e) {
            free(nextCar);
            return;
        }

        e->function = &addLastCar;
        e->car = nextCar;
        e->cooldown = cumulativeCooldown;

        addLastEffectToEffectQueue(gs->effects, e);

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

        // The car's accumulator is a number that once reached will make the car move forward. It is then resets. The speed is the number of accumulating points given to a car per Frame.
        // Therefore, a car with an accumulator or 100, a speed of 2, will move forward every 50 frames. In 60 FPS, it is 1.2 seconds.
        // Formula is : FPS/(Accumulator/Speed). Higher FPS = Slower cars, unless the accumulator is scalled down/speed is scalled up.

        if (c->accumulator == c->speed)
        {
            for (int i = 0; i < c->size; i++)
                if (c->y >= 0 && c->y < gs->grid->height && c->x + i * c->direction >= 0 && c->x + i * c->direction < gs->grid->length)
                    gs->grid->cases[c->y][c->x + i * c->direction] = ROAD;

            int newX = c->x + c->direction;
            c->x = (newX < 0) ? gs->grid->length - 1 : (newX >= gs->grid->length ? 0 : newX);
            c->accumulator = 0;

            for (int i = 0; i < c->size; i++)
                if (c->y >= 0 && c->y < gs->grid->height && c->x + i * c->direction >= 0 && c->x + i * c->direction < gs->grid->length)
                    gs->grid->cases[c->y][c->x + i * c->direction] = c->direction == 1 ? CAR_RIGHT : CAR_LEFT;
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
