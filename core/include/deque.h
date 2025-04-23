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
Car* get_first(Deque* deque);
Car* remove_first(Deque* deque);
Car* remove_last(Deque* deque);
// Car *remove_pos(Deque *deque, int x, int y);
void removeRow(Deque* deque, int y);
void destroy_deque(Deque* queue);
bool is_empty(Deque* deque);
int get_size(Deque* deque);
void add_first(Deque* deque, Car* car);
void add_last(Deque* queue, Car* car);
void print_deque(Deque* queue);

#endif
