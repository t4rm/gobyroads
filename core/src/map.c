#include <stdlib.h>
#include <stdio.h>
#include "map.h"

Grid *createGrid(int height, int length, int carMaxSize)
{
    Grid *grid = (Grid *)malloc(sizeof(Grid));
    grid->height = height;
    grid->length = length + 2 * carMaxSize;
    grid->cases = malloc(sizeof(Occupation *) * height);

    for (int i = 0; i < height; i++)
    {
        Occupation baseTile = i == 0 ? SAFE : ROAD;
        grid->cases[i] = createRow(length + 2 * carMaxSize, baseTile);
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

void displayGrid(Grid *grid, int score, int playerX, int playerY, int carMaxSize) {
    printf("\033[1;1H");
    printf("Crossy Roads | Score : %d | L = %d | H = %d\n\n", score, grid->length, grid->height); 
    printf("\033[3;1H"); 

    for (int i = 0; i < grid->height; i++) {
        int row = grid->height - 1 - i;
    
        for (int j = carMaxSize; j < grid->length - carMaxSize + 1; j++) {
            int virtualJ = j - carMaxSize;
            printf("\033[%d;%dH", i + 3, virtualJ);
    
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