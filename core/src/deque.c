#include <stdlib.h>
#include <stdio.h>
#include "deque.h"
#include "gamestate.h"

Deque *createDeque(void)
{
    Deque *new = malloc(sizeof(Deque));
    new->size = 0;
    new->head = malloc(sizeof(Element));
    new->tail = malloc(sizeof(Element));

    return new;
}

bool is_empty(Deque *deque) { return deque->size == 0; }
int get_size(Deque *deque) { return deque->size; }

void add_first(Deque *deque, Car *car)
{
    Element *new_e = malloc(sizeof(Element));
    new_e->car = car;
    new_e->next = deque->head;
    new_e->previous = NULL;
    deque->head->previous = new_e;
    deque->head = new_e;
    if (deque->size == 0)
        deque->tail = new_e;
    deque->size += 1;
}

Car *get_first(Deque *deque)
{
    if (is_empty(deque))
        return NULL;
    return deque->head->car;
}

void add_last(Deque *queue, Car *car)
{
    Element *new_e = malloc(sizeof(Element));
    new_e->car = car;
    new_e->previous = queue->tail;
    new_e->next = NULL;
    queue->tail->next = new_e;
    queue->tail = new_e;

    if (queue->size == 0)
        queue->head = new_e;
    queue->size += 1;
}

Car *get_last(Deque *queue)
{
    if (is_empty(queue))
        return NULL;
    return queue->tail->car;
}

void destroy_deque(Deque *queue)
{
    Element *cursor = queue->head;
    while (cursor != NULL)
    {
        Element *next = cursor->next;
        if (cursor->car)
            free(cursor->car);
        free(cursor);
        cursor = next;
    }
    free(queue);
}

void print_deque(Deque *queue)
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
        printf("#%d, y : %d\n", i, cursor->car->y);
        cursor = cursor->next;
    }
    printf("]\n");
}

Car *remove_first(Deque *deque)
{
    if (deque->size == 0)
        return NULL;

    Element *tmp = deque->head;
    Car *s = tmp->car;
    deque->head = tmp->next;
    free(tmp);
    deque->head->previous = NULL;
    deque->size--;

    return s;
}

Car *remove_last(Deque *deque)
{
    if (deque->size == 0)
        return NULL;

    Element *tmp = deque->tail;
    Car *s = tmp->car;
    deque->tail = tmp->previous;
    free(tmp);
    deque->tail->next = NULL;
    deque->size--;

    return s;
}