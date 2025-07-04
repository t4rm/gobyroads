#ifndef CAR_QUEUE_H
#define CAR_QUEUE_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "gamestate.h"

typedef struct _Car Car;

typedef struct _CarElement
{
    Car *car;
    struct _CarElement *next;
    struct _CarElement *previous; // Only used by SDL2
} CarElement;

typedef struct _CarQueue
{
    CarElement *head;
    CarElement *tail;
    int size;
} CarQueue;

CarQueue *createCarQueue(void);
void destroyCarQueue(CarQueue *queue);
void removeFirstCar(CarQueue *Que);
void removeRowCar(CarQueue *queue, int y);
void addLastCar(CarQueue *queue, Car *car);
void printCarQueue(CarQueue *queue);

#endif
