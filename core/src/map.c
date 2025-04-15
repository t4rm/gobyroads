#include <stdio.h>
#include <stdlib.h>
#include "gameState.h"
#include "map.h"

Grid *createGrid(int height, int length) {
    Grid *grid = (Grid *) malloc(sizeof(Grid));
    grid->height = height;
    grid->length = length;
    grid->cases = malloc(sizeof(Occupation*) * height);
    
    for (int i = 0; i < height; i++) {
        grid->cases[i] = createRow(length);
    }


    return grid;
}


Occupation *createRow(int length){

    Occupation *row = (Occupation*) malloc(length*sizeof(Occupation));
    
    for(int i = 0; i < length; i++){
        row[i] = SAFE;
    }

    return row;
}

void displayGrid(Grid *grid, Player *player){
    for(int i  = 0; i < grid->height; i++){
        for(int j = 0; j < grid->length; j++){
            if(j == player->posY && i ==  player->posX){
                printf("P");
            }

            switch (grid->cases[i][j]) {
                case SAFE:
                    printf("_");
                    break;
                case ROAD:
                    printf(".");
                    break;
                case CAR:
                    printf("C");
                    break;
                case USER:
                    printf("U");
                    break;
                default:
                    printf("?");
                    break;
            }
        }
        printf("\n\r");
    }
}

