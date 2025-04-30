#include <stdio.h>
#include <windows.h> // Pour Sleep()
// #include <time.h>   // Pour clock_gettime()
// #include <unistd.h> // Pour usleep()
// Inversez les commentaires si vous n'êtes pas sur windows.
#include "gamestate.h"
#include "player.h"
#include "effect.h"
#include "car.h"


int main()
{
    GameState *gs = initGameState(15, 30);
    const int FPS = 60;
    const int frameTime = 1000 / FPS;

    while (!gs->gameOver)
    {
        DWORD frameStartTime = GetTickCount();
        // struct timespec frameStartTime;
        // clock_gettime(CLOCK_MONOTONIC, &frameStartTime); // Récupérer l'heure en nanosecondes

        // if (gs->player->afk >= FPS * 4) gs->gameOver = true;
        
        // Start of the game handling logic
        updateCars(gs);
        updateEffects(gs);
        updateGameState(gs);
        // Game is updated, map is fresh, cars progressed
        playerMove(gs);
        handleCollision(gs);
        // Player moved, if he collided the game stops, otherwise we scroll down when y >= 3.
        scrolling(gs);
        // End of the game handling logic.

        DWORD frameEndTime = GetTickCount();
        DWORD elapsedTime = frameEndTime - frameStartTime;

        if (elapsedTime < frameTime) {
            DWORD sleepTime = frameTime - elapsedTime;
            Sleep(sleepTime); 
        }

        // struct timespec frameEndTime;
        // clock_gettime(CLOCK_MONOTONIC, &frameEndTime);

        // long long elapsedTime = (frameEndTime.tv_sec - frameStartTime.tv_sec) * 1000 +
        //                         (frameEndTime.tv_nsec - frameStartTime.tv_nsec) / 1000000; // Temps écoulé en millisecondes

        // // Si l'exécution de la frame prend moins de temps que frameTime, on fait une pause
        // if (elapsedTime < frameTime) {
        //     long long sleepTime = frameTime - elapsedTime;
        //     usleep(sleepTime * 1000); // Attendre le temps nécessaire (en microsecondes)
        // }
    }

    printf("\e[1;1H\e[2J");

    if (gs->gameOver) printf("\n\nPerdu, score final: %d\n", gs->score);

    destroyGameState(gs);

    return 0;
}
