#include <stdio.h>
#include <stdlib.h>
#include "map.h"

Row *createGrid(int length, int height){
    Row *head = NULL;
    Row *tail = NULL;

    for(int i = 0; i < height; i++){
        Row *row = (Row*) malloc(sizeof(Row));

        row->next = NULL;
        row->cases = createRow(length);

        if (head == NULL) {
            head = row;
            tail = row;
        } else {
            tail->next = row;
            tail = row;
        }
    }


    
    return head;

}

Occupation *createRow(int length){

    Occupation *row = (Occupation*) malloc(length*sizeof(Occupation));

    for(int i = 0; i < length; i++){
        row[i] = SAFE;
    }

    return row;
}

void displayGrid(Row *row, int length){


    while(row != NULL){
        for(int i = 0; i < length; i++){
            switch (row->cases[i])
            {
            case SAFE:
                printf("_");
                break;
            
            default:
                break;
            }
        }
        printf("\n\r");
        row = row->next;
    }
}


