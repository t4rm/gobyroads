#include <windows.h> // Pour Sleep()
#include <stdio.h>
#include "gameState.h"

int main()
{
    GameState *gs = initGameState(15, 30);
    const int FPS = 12; 
    const int frameDelay = 1000 / FPS;

    while (!gs->gameOver)
    {
        int frameStart = GetTickCount();
        
        updateGameState(gs);
        playerMove(gs);
        scrolling(gs);
        handleCollision(gs);
        
        int frameTime = GetTickCount() - frameStart;

        if (frameDelay > frameTime)
        {
            Sleep(frameDelay - frameTime);
        }
    }

    if (gs->gameOver) printf("\n\nPerdu, score final: %d\n", gs->score);

    return 0;
}