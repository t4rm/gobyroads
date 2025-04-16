#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include "gamestate.h"

GameState *initGameState(int h, int l)
{
    GameState *gs = (GameState *)malloc(sizeof(GameState));
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
    
    return gs;
}

void updateGameState(GameState *gs)
{
    printf("\e[1;1H\e[2J");
    printf("\nScore: %d\n", gs->score);

    displayGrid(gs->grid, gs->player->x, gs->player->y);
}

void playerMove(GameState *gs)
{
    if (_kbhit())
    {
        char key = _getch();

        switch (key)
        {
        case 'z':
            if (gs->player->y < gs->grid->height - 1) {
                gs->player->y++;
                gs->score++;
            }
            break;
        case 's': // Move down
            if (gs->player->y > 0) {
                gs->player->y--;
                gs->score--;
            }
            else gs->gameOver = true; // 3 pas en arrière.
            break;
        case 'q': // Move left
            if (gs->player->x > 0)
                gs->player->x--;
            break;
        case 'd': // Move right
            if (gs->player->x < gs->grid->length - 1)
                gs->player->x++;
            break;
        case 'f':
            gs->gameOver = true;
            break;
        }
    }
}

void scrolling(GameState *gs)
{
    // Si le joueur avance de 3 lignes
    if (gs->player->y == 3)
    {
        // Décale toutes les lignes vers le bas
        for (int i = 0; i < gs->grid->height; i++)
        {
            gs->grid->cases[i] = gs->grid->cases[i + 1];
        }

        // Crée une nouvelle ligne en haut (par défaut, des routes)
        gs->grid->cases[gs->grid->height - 1] = createRow(gs->grid->length, ROAD);

        // Réinitialise la position du joueur sur la grille
        gs->player->y -= 1;

        // Décremente la distance avant la prochaine safezone.
        gs->nextSafeZone--;

        // Vérifie si une nouvelle safe zone doit être générée
        if (gs->nextSafeZone == 0) {
            applyOccupationToRow(gs->grid->cases[gs->grid->height-1], gs->grid->length, SAFE);
            gs->nextSafeZone = 4 + (gs->score / 10);
        }
    }
}

// handleCollision(GameState *gs) {

// }