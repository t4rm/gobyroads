#include <stdio.h>
#include <windows.h> // Pour Sleep()
#include <time.h>
// #include <unistd.h> // Pour usleep()
// Inversez les commentaires si vous n'êtes pas sur windows.
#include "gamestate.h"
#include "player.h"
#include "car.h"
#include "agent_ai.h"
#include "a_star.h"

int main()
{
    srand(time(NULL));
    GameState *gs = initGameState(15, 32 - 12);
    const int FPS = 60;
    const int frameTime = 1000 / FPS;
    bool AI = true;
    int pathLength = 0;
    int cmpt = 0;
    Node **path = NULL;

    while (!gs->gameOver)
    {
        DWORD frameStartTime = GetTickCount();

        if (gs->player->afk >= FPS * 6)
            gs->gameOver = true;

        // AI -------------------------
        if (AI)
            aiLoop(gs, &pathLength, &cmpt, &path);
        else
            playerMove(gs);
        // -------------------------------

        // Start of the game handling logic
        updateCars(gs);
        updateGameState(gs);
        // Game is updated, map is fresh, cars progressed

        updateIce(gs);
        updateTrain(gs->grid);
        handleCollision(gs);
        handleScore(gs);
        // Player moved, if he collided the game stops, otherwise we scroll down when y >= 3.
        scrolling(gs, AI);
        // End of the game handling logic.

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
    printf("\e[?25h"); // Afficher le curseur

    return 0;
}
