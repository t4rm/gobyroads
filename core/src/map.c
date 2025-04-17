#include <stdlib.h>
#include <stdio.h>
#include "map.h"

Grid *createGrid(int height, int length)
{
    Grid *grid = (Grid *)malloc(sizeof(Grid));
    grid->height = height;
    grid->length = length;
    grid->cases = malloc(sizeof(Occupation *) * height);

    for (int i = 0; i < height; i++)
    {
        Occupation baseTile = i == 0 ? SAFE : ROAD;
        grid->cases[i] = createRow(length, baseTile);
    }

    return grid;
}

Occupation *createRow(int length, Occupation type)
{
    Occupation *row = (Occupation *)malloc(length * sizeof(Occupation));
    applyOccupationToRow(row, length, type);
    return row;
}

void applyOccupationToRow(Occupation *row, int length, Occupation type) {
    for (int i = 0; i < length; i++) row[i] = type == SAFE ? rand() % length/3 ? SAFE : TREE : type;
}

void displayGrid(Grid *grid, int score, int playerX, int playerY) {
    printf("\033[1;1H");
    printf("Crossy Roads | Score : %d\n\n", score); 
    printf("\033[3;1H"); 

    for (int i = 0; i < grid->height; i++) {
        int row = grid->height - 1 - i;
    
        for (int j = 0; j < grid->length; j++) {
            printf("\033[%d;%dH", i + 3, j + 1);
    
            if (row == playerY && j == playerX) {
                printf("P");
            } else {
                switch (grid->cases[row][j]) {
                    case SAFE: printf("="); break;
                    case CAR: printf("V"); break;
                    case ROAD: printf("_"); break;
                    case TREE: printf("T"); break;
                    default: printf("?"); break;
                }
            }
        }
    }

    fflush(stdout);
}