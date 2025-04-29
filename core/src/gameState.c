#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include "gamestate.h"
#include "deque.h"

GameState *initGameState(int h, int l)
{
    GameState *gs = (GameState *)malloc(sizeof(GameState));
    gs->carMaxSize = 6;
    gs->cars = createDeque();
    gs->effects = createQue();
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
    destroyDeque(gs->cars);
    destroyQue(gs->effects);
    destroyGrid(gs->grid);
    free(gs->player);
    free(gs);
}

void playerMove(GameState *gs)
{
    if (_kbhit())
    {
        char key = _getch();
        bool hasMoved = false;
        int precedentX = gs->player->x, precedentY = gs->player->y;

        if (gs->player->mouvementCooldown == 0)
        {
            switch (key)
            {
            case 'z':
                if (gs->player->y < gs->grid->height - 1)
                {
                    gs->player->y++;
                    gs->score++;
                    hasMoved = true;
                }
                break;
            case 's': 
                if (gs->player->y > 0)
                {
                    gs->player->y--;
                    gs->score--;
                    hasMoved = true;
                }
                else
                    gs->gameOver = true; // 3 pas en arrière.
                break;
            case 'q': 
                if (gs->player->x > gs->carMaxSize)
                {
                    gs->player->x--;
                    hasMoved = true;
                }
                break;
            case 'd': 
                if (gs->player->x < gs->grid->length - 1 - gs->carMaxSize)
                {
                    gs->player->x++;
                    hasMoved = true;
                }
                break;
            case 'f':
                gs->gameOver = true;
                break;
            }
        }

        // Mouvement non autorisé.
        if (gs->grid->cases[gs->player->y][gs->player->x] == TREE) {
            gs->player->x = precedentX;
            gs->player->y = precedentY;
            gs->player->mouvementCooldown = 1; // Cooldown moins important en cas de mouvement non autorisé : A confirmer 
        }

        if (hasMoved) {
            gs->player->mouvementCooldown = 3; // 2 mouvements par seconde autorisé : À confirmer
            gs->player->afk = 0;
        }
    }

    gs->player->mouvementCooldown = gs->player->mouvementCooldown <= 1 ? 0 : gs->player->mouvementCooldown - 1;
    gs->player->afk++;
}

void scrolling(GameState *gs)
{
    if (gs->player->y == 3)
    {
        if (gs->grid->cases[0][0] != SAFE && gs->grid->cases[0][0] != TREE) {
            removeRow(gs->cars, 0);
            removeRowEffect(gs->effects, 0);
        }

        for (int i = 0; i < gs->grid->height - 1; i++)
            gs->grid->cases[i] = gs->grid->cases[i + 1];

        gs->grid->cases[gs->grid->height - 1] = createRow(gs->grid->length, ROAD);

        gs->player->y -= 1;

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
    
    Element *cursor = gs->cars->head;
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
    addLastDeque(gs->cars, baseCar);

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
        if (!e) return;

        e->function = &addLastDeque;
        e->car = nextCar;
        e->cooldown = cumulativeCooldown;

        addLastEffectToQue(gs->effects, e);

        lastSize = nextSize;
        availableSize -= nextSize;
        desiredCars--;
    }
}

void updateEffects(GameState *gs)
{
    EffectElement *cursor = gs->effects->head;
    if (cursor == NULL)
        return;

    if (cursor->effect->car->y >= 0 && cursor->effect->cooldown <= 0)
    {
        cursor->effect->function(gs->cars, cursor->effect->car);
        removeFirstEffect(gs->effects);
    }
    else
    {
        while (cursor != NULL)
        {
            cursor->effect->cooldown--;
            cursor = cursor->next;
        }
    }
}

void updateCars(GameState *gs)
{
    Element *cursor = gs->cars->head;
    while (cursor != NULL)
    {
        Car *c = cursor->car;
        Element *next = cursor->next;

        if (c->accumulator == c->speed)
        {
            for (int i = 0; i < c->size; i++)
                if (c->y >= 0 && c->y < gs->grid->height && c->x + i * c->direction >= 0 && c->x + i * c->direction < gs->grid->length)
                    gs->grid->cases[c->y][c->x + i * c->direction] = ROAD;

            int newX = c->x + c->direction;

            if (newX < 0)
                c->x = gs->grid->length - 1;
            else if (newX >= gs->grid->length)
                c->x = 0;
            else
                c->x = newX;

            c->accumulator = 0;

            for (int i = 0; i < c->size; i++)
                if (c->y >= 0 && c->y < gs->grid->height && c->x + i * c->direction >= 0 && c->x + i * c->direction < gs->grid->length)
                    gs->grid->cases[c->y][c->x + i * c->direction] = c->direction == 1 ? CAR_RIGHT : CAR_LEFT;
            
        }
        else
            c->accumulator++;

        cursor = next;
    }
}

void decrementCarsOnY(GameState *gs)
{
    Element *cursor = gs->cars->head;
    while (cursor != NULL)
    {
        Car *c = cursor->car;
        c->y--;
        cursor = cursor->next;
    }
}

void decrementEffectsOnY(GameState *gs)
{
    EffectElement *cursor = gs->effects->head;
    EffectElement *prev = NULL;

    while (cursor != NULL)
    {
        Car *c = cursor->effect->car;
        c->y--;

        // Supprime les effets avec y < 0
        if (c->y < 0)
        {
            EffectElement *toDelete = cursor;
            if (prev == NULL) {
                gs->effects->head = cursor->next;
            } else {
                prev->next = cursor->next;
            }

            cursor = cursor->next;

            free(toDelete->effect);
            free(toDelete);
            gs->effects->size--;

            continue;
        }

        prev = cursor;
        cursor = cursor->next;
    }
}

void updateGameState(GameState *gs)
{
    updateCars(gs);
    displayGrid(gs->grid, gs->score, gs->player->x, gs->player->y, gs->carMaxSize);
    // printDeque(gs->cars); // Pour faire de la debug
    // printQue(gs->effects);
    // printf("%d, %d", gs->player->x, gs->player->y);
}

