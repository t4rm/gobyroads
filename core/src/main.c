#include <stdio.h>
#include <windows.h> // Pour Sleep()
// #include <time.h>
// #include <unistd.h> // Pour usleep()
// Reverse the comments if you aren't on Windows.
#include "gamestate.h"
#include "player.h"
#include "car.h"
#include "agent_ai.h"
#include "a_star.h"

int main()
{
    GameState *gs = initGameState(15, 32 - 12);
    const int FPS = 60;
    const int frameTime = 1000 / FPS;
    int pathLength = 0;
    int cmpt = 0;
    Node **path = NULL;
    bool AI = true;

    while (!gs->gameOver)
    {
        DWORD frameStartTime = GetTickCount(); // Get the current time.

        if (gs->player->afk >= FPS * 6)
            gs->gameOver = true;

        // Start of the game handling logic
        updateCars(gs);
        updateGameState(gs);
        // Game is updated, map is fresh, cars progressed
        // AI -------------------------
        if (AI)
        {
            gs->player->mouvementCooldown = gs->player->mouvementCooldown <= 1 ? 0 : gs->player->mouvementCooldown - 1;
            gs->player->afk++;

            if (cmpt >= pathLength)
            {
                path = getPathAI(gs, &pathLength);
                cmpt = 0;
            }
            if (cmpt < pathLength)
            {
                if (gs->player->mouvementCooldown == 0)
                {
                    playerMoveAi(gs, path[cmpt]);
                    cmpt++;
                }
            }
        }
        else
            playerMove(gs);
        // -------------------------------

        updateIce(gs);
        updateTrain(gs->grid);
        handleCollision(gs);
        handleScore(gs);
        // Player moved, if he collided the game stops, otherwise we scroll down when y >= 3.
        scrolling(gs, AI);
        // End of the game handling logic.

        // We compare the time at the start and the end of the frame, if the elapsedTime is inferior to the desired frameTime we wait until it is reached.
        DWORD frameEndTime = GetTickCount();
        DWORD elapsedTime = frameEndTime - frameStartTime;

        if (elapsedTime < frameTime)
        {
            DWORD sleepTime = frameTime - elapsedTime;
            Sleep(sleepTime);
        }
    }

    printf("\e[1;1H\e[2J");

    if (gs->gameOver)
        printf("\n\nPerdu, score final: %d\n", gs->score);

    destroyGameState(gs);
    printf("\e[?25h"); // Show back the cursor

    return 0;
}
