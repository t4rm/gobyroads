#ifndef GAME_STATE
#define GAME_STATE

#include "map.h" 
#include "stdbool.h"

typedef struct Player{
    int posX;
    int posY;
}Player;

typedef struct GameState
{
    Player* player;
    int time;
    Row *grid;
}GameState;

bool playerMove(GameState* p_gameState);
GameState *init_gameState(); 
void update_gameState();

#endif 
