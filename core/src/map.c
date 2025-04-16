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
    for (int i = 0; i < length; i++) row[i] = type;
}

void displayGrid(Grid *grid, int playerX, int playerY)
{
    for (int i = grid->height-1; i >= 0; i--)
    {
        for (int j = 0; j < grid->length; j++)
        {
            if (j == playerX && i == playerY) printf("P");
            else switch (grid->cases[i][j])
            {
                case SAFE: printf("="); break;
                case CAR: printf("V"); break;
                case ROAD: printf("_"); break;
                case TREE: printf("T"); break;
                default: printf("?"); break;
            }
        }
        printf("\n");
    }
}
