#include <stdio.h>
#include <stdlib.h>
#include "gameState.h"

int main()
{

    
    GameState *gs = init_gameState();

    while (true)
    {
        update_gameState(gs);
    }
    

    return 0;
}