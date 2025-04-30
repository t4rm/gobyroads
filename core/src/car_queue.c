#include <stdlib.h>
#include <stdio.h>
#include "car_queue.h"

CarQueue *createCarQueue(void)
{
    CarQueue *queue = malloc(sizeof(CarQueue));
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

void destroyCarQueue(CarQueue *queue)
{
    CarElement *cursor = queue->head;
    while (cursor != NULL)
    {
        CarElement *next = cursor->next;
        if (cursor->car) free(cursor->car);
        free(cursor);
        cursor = next;
    }
    free(queue);
}

void removeFirstCar(CarQueue *queue)
{
    if (queue->size == 0) return;

    CarElement *tmp = queue->head;
    queue->head = tmp->next;
    if (tmp->car) free(tmp->car);
    free(tmp);
    queue->size--;

    if (queue->size == 0)
        queue->tail = NULL;
}

void removeRowCar(CarQueue *queue, int y)
{
    if (queue->head == NULL) return;

    CarElement *cursor = queue->head;
    CarElement *prev = NULL;

    while (cursor != NULL)
    {
        CarElement *next = cursor->next;

        if (cursor->car && cursor->car->y == y)
        {
            if (prev == NULL)
                queue->head = next;
            else
                prev->next = next;

            if (cursor == queue->tail)
                queue->tail = prev;

            free(cursor->car);
            free(cursor);
            queue->size--;
        }
        else
        {
            prev = cursor;
        }

        cursor = next;
    }
}

void addLastCar(CarQueue *queue, Car *car)
{
    CarElement *new_e = malloc(sizeof(CarElement));
    new_e->car = car;
    new_e->next = NULL;

    if (queue->tail)
        queue->tail->next = new_e;
    else
        queue->head = new_e;

    queue->tail = new_e;
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
    CarElement *cursor = queue->head;

    printf("[\n");
    for (int i = 0; i < queue->size && cursor != NULL; i++)
    {
        if (cursor->car)
        {
            printf("#%d, y : %d, x : %d, s : %d, d : %d, v : %d\n",
                   i, cursor->car->y, cursor->car->x, cursor->car->size,
                   cursor->car->direction, cursor->car->speed);
        }
        cursor = cursor->next;
    }
    printf("]\n");
}

