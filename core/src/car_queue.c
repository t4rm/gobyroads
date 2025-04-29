#include <stdlib.h>
#include <stdio.h>
#include "car_queue.h"
#include "gamestate.h"

CarQueue *createCarQueue(void)
{
    CarQueue *new = malloc(sizeof(CarQueue));
    new->size = 0;
    new->head = NULL;
    new->tail = NULL;
    return new;
}

void removeFirstCar(CarQueue *que)
{
    if (que->size == 0)
    return;
    
    CarElement *tmp = que->head;
    que->head = tmp->next;
    que->size--;
    free(tmp);
}

void removeRowCar(CarQueue *queue, int y)
{
    if (queue->head == NULL)
    return;
    
    CarElement *cursor = queue->head;
    CarElement *prev = NULL;
    
    while (cursor != NULL)
    {
        if (cursor->car->y == y)
        {
            if (prev == NULL)
            {
                queue->head = cursor->next;
            }
            else
            {
                prev->next = cursor->next;
            }
            
            free(cursor->car);
            free(cursor);
            
            queue->size--;
        }
        
        prev = cursor;
        cursor = cursor->next;
    }
}

void addLastCar(CarQueue *queue, Car *car)
{
    CarElement *new_e = malloc(sizeof(CarElement));
    new_e->car = car;
    new_e->next = NULL;
    queue->tail->next = new_e;
    queue->tail = new_e;

    if (queue->size == 0)
        queue->head = new_e;

    queue->size++;
}

void printCarQueue(CarQueue *queue)
{
    if (queue->size == 0)
    {
        printf("[\n]\n");
        return;
    }
    
    printf("%d\n", queue->size);
    
    Element *cursor = queue->head;
    
    printf("[\n");
    for (int i = 0; i < queue->size; i++)
    {
        printf("#%d, y : %d, x : %d, s : %d, d : %d, v : %d\n", i, cursor->car->y, cursor->car->x, cursor->car->size, cursor->car->direction, cursor->car->speed);
        cursor = cursor->next;
    }
    printf("]\n");
}

void destroyCarQueue(CarQueue *queue)
{
    CarElement *cursor = queue->head;
    while (cursor != NULL)
    {
        CarElement *next = cursor->next;
        if (cursor->car)
            free(cursor->car);
        free(cursor);
        cursor = next;
    }
    free(queue);
}
