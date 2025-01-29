#include <stdio.h>
#include <stdlib.h>
#include "map.h"

Grid initializeGrid(int rows, int columns)
{
    Grid g;
    g.rows = rows;
    g.cols = columns;
    g.values = malloc(sizeof(int) * rows * columns);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            SET(&g, i, j, ROAD);
        }
    }
    return g;
}

void freeGrid(Grid *g) { free(g->values); }

void printGrid(Grid *g)
{
    printf("[\n");
    for (int i = 0; i < g->rows; i++)
    {
        printf("    [");
        for (int j = 0; j < g->cols; j++)
        {
            printf("%d", GET(g, i, j));
            if (j < g->cols - 1)
                printf(", ");
        }
        printf("]");
        if (i < g->rows - 1)
            printf(",");
        printf("\n");
    }
    printf("]\n");
}
