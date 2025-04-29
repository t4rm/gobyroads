#ifndef DEQUE_H
#define DEQUE_H
#include "gamestate.h"
#include <stdbool.h>

typedef struct _Car Car;

typedef struct _Element {
    Car * car;
    struct _Element * next;
    struct _Element * previous;
} Element;

typedef struct _Deque {
    Element * head;
    Element * tail;
    int size;
} Deque;

Deque* createDeque(void);
Car* getFirstDeque(Deque* deque);
Car* removeFirstDeque(Deque* deque);
Car* removeLastDeque(Deque* deque);
// Car *remove_pos(Deque *deque, int x, int y);
void removeRow(Deque* deque, int y);
void destroyDeque(Deque* queue);
bool isEmptyDeque(Deque* deque);
int getSizeDeque(Deque* deque);
void addFirstDeque(Deque* deque, Car* car);
void addLastDeque(Deque* queue, Car* car);
void printDeque(Deque* queue);

#endif
