#include <stdio.h>
#include <windows.h> // Pour Sleep()
// #include <time.h>   // Pour clock_gettime()
// #include <unistd.h> // Pour usleep()
// Inversez les commentaires si vous n'êtes pas sur windows.
#include "gamestate.h"
#include "player.h"
#include "car.h"

int main()
{
    GameState *gs = initGameState(15, 32 - 12);
    const int FPS = 60;
    const int frameTime = 1000 / FPS;

    while (!gs->gameOver)
    {
        DWORD frameStartTime = GetTickCount();

        if (gs->player->afk >= FPS * 6)
            gs->gameOver = true;

        // Start of the game handling logic
        updateCars(gs);
        updateGameState(gs);
        // Game is updated, map is fresh, cars progressed
        playerMove(gs);
        updateIce(gs);
        updateTrain(gs->grid);
        handleCollision(gs);
        handleScore(gs);
        // Player moved, if he collided the game stops, otherwise we scroll down when y >= 3.
        scrolling(gs);
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
