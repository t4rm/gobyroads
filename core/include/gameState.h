#ifndef GAME_STATE
#define GAME_STATE

#include "car.h"
#include "map.h" 
#include "stdbool.h"

typedef struct _Player{
    int posX;
    int posY;
    int mouvement_cooldown;
}Player;

typedef struct _GameState
{
    Player* player;
    int time;
    Row *grid;
    int score;
    Car cars[100];              // A voir
    int cars_amount;
    int cars_cooldown;
}GameState;

bool playerMove(GameState* p_gameState);
GameState *init_gameState(); 
void update_gameState();
void scrolling(GameState* gameState);
bool check_collision(GameState *gameState);

#endif 
