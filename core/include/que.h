#ifndef QUE_H
#define QUE_H
#include "gamestate.h"
#include <stdbool.h>

typedef struct _Deque Deque;
typedef struct _Car Car;

typedef struct _Effect {
    int cooldown;
    void (*function)(Deque *queue, Car *car);
    Car * car;
  } Effect;

typedef struct _EffectElement {
    Effect * effect;
    struct _EffectElement * next;
} EffectElement;

typedef struct _Que {
    EffectElement * head;
    EffectElement * tail;
    int size;
} Que;

Que* createQue(void);
Effect* getFirstEffect(Que* Que);
void* removeFirstEffect(Que* Que);
bool isEmptyQue(Que* Que);
int getQueSize(Que* Que);
void addLastEffectToQue(Que* queue, Effect* effect);
void destroyQue(Que* queue);
void printQue(Que* queue);
void *removeRowEffect(Que *queue, int y);

#endif
