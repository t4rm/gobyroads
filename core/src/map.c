#include <stdlib.h>
#include <stdio.h>
#include "map.h"

/* Create a virtual grid, the viewable grid is the virtual grid but with margins.
 * height, length being the dimensions of the grid
 * carMaxSize, the length of a margin which should be able to host any car, therefore being able to host the biggest car possible.
 */
Grid *createGrid(int height, int length, int carMaxSize)
{
    int l = length + 2 * carMaxSize;
    Grid *grid = (Grid *)malloc(sizeof(Grid));
    grid->height = height;
    grid->length = l;
    grid->cases = malloc(sizeof(Occupation *) * height);
    grid->rowManagers = malloc(sizeof(RowManager *) * height);

    for (int i = 0; i < height; i++)
    {
        Occupation baseTile = i == 0 ? SAFE : ROAD;
        grid->cases[i] = createRow(l, baseTile);
        grid->rowManagers[i] = createRowManager(rand() % 2 ? 1 : -1, 25 + (rand() % (60 - 25 + 1)), baseTile);
    }

    return grid;
}
/* create a RowManager
 * direction, speed and type being the parameters of the new Row
 * type can be ROAD/WATER/RAILS
 */
RowManager *createRowManager(int direction, int speed, Occupation type)
{
    RowManager *rowManager = (RowManager *)malloc(sizeof(RowManager));
    rowManager->direction = direction;
    if (type == RAIL)
        rowManager->cooldown = rand() % speed;
    else
        rowManager->cooldown = speed;
    rowManager->speed = speed;
    rowManager->type = type;
    return rowManager;
}

/* create a grid Row
 * length and type being the properties of this row
 */
Occupation *createRow(int length, Occupation type)
{
    Occupation *row = (Occupation *)malloc(length * sizeof(Occupation));
    applyOccupationToRow(row, length, type);
    return row;
}

/* free a grid and its rows
 * g: pointer to the grid
 */
void destroyGrid(Grid *g)
{
    for (int i = 0; i < g->height; i++)
        free(g->cases[i]);
    free(g);
}

/* apply an Occupation to a row
 * row : the row to edit
 * length : the length of this row
 * type : the Occupation to apply
 */
void applyOccupationToRow(Occupation *row, int length, Occupation type)
{
    if (type == SAFE)
        createTrees(row, length);
    else
        for (int i = 0; i < length; i++)
            row[i] = type;
}

/* create trees in a random way on a Row
 * row, length: the row and its length
 */
void createTrees(Occupation *row, int length)
{
    int difficulty = 25;
    for (int i = 0; i < length; i++)
        row[i] = TREE;
    for (int i = 0; i < difficulty; i++)
        row[rand() % length] = SAFE;
}

/* rewrite the grid to display it 
 * grid: a pointer to the grid
 * score: the score to show
 * playerX, playerY: the coordinates of the player
 * carMaxSize: the length of a margin
 */
void displayGrid(Grid *grid, int score, int playerX, int playerY, int carMaxSize)
{
    printf("\033[1;1H");
    printf("Crossy Roads | Score : %d | Appuyer sur \"f\" pour quitter. \n\n", score);
    printf("\033[3;1H");

    for (int i = 0; i < grid->height; i++)
    {
        int row = grid->height - 1 - i;

        for (int j = carMaxSize; j < grid->length - carMaxSize + 1; j++)
        {
            int virtualJ = j - carMaxSize;
            printf("\033[%d;%dH", i + 3, virtualJ);

            if (row == playerY && j == playerX)
            {
                printf("P");
            }
            else
            {
                switch (grid->cases[row][j])
                {
                case ROAD:
                    printf("_");
                    break;
                case WATER:
                    printf("~");
                    break;
                case SAFE:
                    printf("=");
                    break;
                case TREE:
                    printf("T");
                    break;
                case CAR_LEFT:
                    printf("<");
                    break;
                case CAR_RIGHT:
                    printf(">");
                    break;
                case LOG:
                    printf("L");
                    break;
                case ICE:
                    printf("I");
                    break;
                case RAIL:
                    printf("R");
                    break;
                case WARNING:
                    printf("!");
                    break;
                case TRAIN:
                    printf("O");
                    break;
                default:
                    printf("?");
                    break;
                }
            }
        }
    }

    fflush(stdout);
}