#include "gameState.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define car_cooldown 10

GameState *init_gameState()
{
    Row *grid = createGrid(8, 10);
    GameState *gs = (GameState *)malloc(sizeof(GameState));

    Player *player = (Player *)malloc(sizeof(Player));
    player->posX = 4;
    player->posY = 5;

    gs->score = 0;
    gs->cars_amount = 0;
    gs->cars_cooldown = car_cooldown;
    gs->grid = grid;
    gs->player = player;
    gs->time = 0;
    return gs;
}

void update_gameState(GameState *gs)
{

    displayGrid(gs->grid, 8, gs->player);
}

bool playerMove(GameState *p_gameState)
{

    char input = getchar();

    if (input != 'z' && input != 'q' && input != 's' && input != 'd' && input != 'h')
    {
        printf("Use ZQSD to move !\n");
        input = getchar();
    }
    if (input == 'h')
    {
        printf("<put help here>\n");
        input = getchar();
    }

    switch (input)
    {
    case 'z':
        p_gameState->player->posY--;
        break;
    case 'q':
        p_gameState->player->posX--;
        break;
    case 's':
        p_gameState->player->posY++;
        break;
    case 'd':
        p_gameState->player->posX++;
        break;
    }

    return true;
}
