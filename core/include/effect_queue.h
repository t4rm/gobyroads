#ifndef EFFECT_QUEUE_H
#define EFFECT_QUEUE_H

#include <stdbool.h>
#include "gamestate.h"

typedef struct _CarQueue CarQueue;
typedef struct _Car Car;

typedef struct _Effect
{
    int cooldown;
    void (*function)(CarQueue *queue, Car *car);
    Car *car;
} Effect;

typedef struct _EffectElement
{
    Effect *effect;
    struct _EffectElement *next;
} EffectElement;

typedef struct _EffectQueue
{
    EffectElement *head;
    EffectElement *tail;
    int size;
} EffectQueue;

EffectQueue *createEffectQueue(void);
void removeFirstEffect(EffectQueue *Que);
void destroyEffect(EffectElement *element);
void addLastEffectToEffectQueue(EffectQueue *queue, Effect *effect);
void printEffectQueue(EffectQueue *queue);
void removeRowEffect(EffectQueue *queue, int y);
void destroyEffectQueue(EffectQueue *queue);

#endif
