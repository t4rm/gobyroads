#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include "gamestate.h"
#include "deque.h"

GameState *initGameState(int h, int l)
{
    GameState *gs = (GameState *)malloc(sizeof(GameState));
    gs->cars = createDeque();
    gs->grid = createGrid(h, l);
    gs->player = (Player *)malloc(sizeof(Player));
    gs->player->x = l / 2;
    gs->player->y = 0;
    gs->player->mouvementCooldown = 0;
    gs->score = 0;
    gs->backwardMovements = 0;
    gs->gameOver = false;
    gs->nextSafeZone = 3;

    while (gs->nextSafeZone < h)
    {
        applyOccupationToRow(gs->grid->cases[gs->nextSafeZone], l, SAFE);
        gs->nextSafeZone += 4 + (gs->score / 10);
    }

    gs->nextSafeZone = 3 + (gs->score / 10);

    for (int i = 0; i < gs->grid->height; i++)
        addCar(gs, i, 0);

    gs->grid->cases[gs->player->y][gs->player->x] = SAFE; // On spawn sur une safe zone, pas un arbre.

    return gs;
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
            case 's': // Move down
                if (gs->player->y > 0)
                {
                    gs->player->y--;
                    gs->score--;
                    hasMoved = true;
                }
                else
                    gs->gameOver = true; // 3 pas en arrière.
                break;
            case 'q': // Move left
                if (gs->player->x > 0)
                {
                    gs->player->x--;
                    hasMoved = true;
                }
                break;
            case 'd': // Move right
                if (gs->player->x < gs->grid->length - 1)
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
            gs->player->mouvementCooldown = 3; // COoldown moins important en cas de mouvement non autorisé : A confirmer 
        }

        if (hasMoved)
            gs->player->mouvementCooldown = 6; // 2 mouvements par seconde autorisé : À confirmer
    }

    gs->player->mouvementCooldown = gs->player->mouvementCooldown <= 1 ? 0 : gs->player->mouvementCooldown - 1;
}

void scrolling(GameState *gs)
{
    if (gs->player->y == 3)
    {
        if (gs->grid->cases[0][0] != SAFE && gs->grid->cases[0][0] != TREE)
            remove_first(gs->cars);

        for (int i = 0; i < gs->grid->height - 1; i++)
            gs->grid->cases[i] = gs->grid->cases[i + 1];

        gs->grid->cases[gs->grid->height - 1] = createRow(gs->grid->length, ROAD);

        gs->player->y -= 1;

        decrementCarsOnY(gs);

        gs->nextSafeZone--;

        if (gs->nextSafeZone == 0)
        {
            applyOccupationToRow(gs->grid->cases[gs->grid->height - 1], gs->grid->length, SAFE);
            gs->nextSafeZone = 4 + (gs->score / 10);
        }
        else
            addCar(gs, gs->grid->height - 1, 0);
    }
}

bool handleCollision(GameState *gs)
{
    if (gs->grid->cases[gs->player->y][0] == SAFE)
        return false;
    
    Element *cursor = gs->cars->head;
    while (cursor != NULL)
    {
        if (cursor->car->x == gs->player->x && cursor->car->y == gs->player->y)
        {
            gs->gameOver = true;
            return true;
        }
        else
            cursor = cursor->next;
    }
    return false;
}

void addCar(GameState *gs, int y, int forcedDirection)
{
    if (y < 0 || y >= gs->grid->height)
        return;
    if (gs->grid->cases[y][0] == SAFE || gs->grid->cases[y][0] == TREE)
        return;

    Car *newCar = (Car *)malloc(sizeof(Car));
    if (!newCar)
        return;

    int direction = forcedDirection == 0 ? (rand() % 2 ? 1 : -1) : forcedDirection;
    int startingX = (direction == 1) ? 0 : gs->grid->length - 1;
    int maxSize = (gs->score / 10 + 3 >= 6) ? 6 : gs->score / 10 + 3;
    int size = rand() % maxSize;
    int speed = rand() % 6;

    *newCar = (Car){.x = startingX, .y = y, .size = size, .direction = direction, .speed = speed, .accumulator = 0};
    gs->grid->cases[y][startingX] = CAR;
    add_last(gs->cars, newCar);
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
            if (c->y >= 0 && c->y < gs->grid->height && c->x >= 0 && c->x < gs->grid->length)
                gs->grid->cases[c->y][c->x] = ROAD;

            int newX = c->x + c->direction;

            if (newX < 0 || newX >= gs->grid->length)
                c->x = newX < 0 ? gs->grid->length - 1 : 0;
            else
                c->x = newX;

            c->accumulator = 0;

            if (c->y >= 0 && c->y < gs->grid->height && c->x >= 0 && c->x < gs->grid->length)
                gs->grid->cases[c->y][c->x] = CAR;
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

void updateGameState(GameState *gs)
{
    updateCars(gs);
    printf("\e[1;1H\e[2J");
    printf("\nScore: %d\n", gs->score);
    displayGrid(gs->grid, gs->player->x, gs->player->y);
    // print_deque(gs->cars); // For debug purposes
}