#include <stdlib.h>
#include <stdio.h>
#include "que.h"
#include "gamestate.h"

Que *createQue(void)
{
    Que *new = malloc(sizeof(Que));
    new->size = 0;
    new->head = malloc(sizeof(EffectElement));
    new->tail = malloc(sizeof(EffectElement));

    return new;
}

bool isEmptyQue(Que *que) { return que->size == 0; }
int getQueSize(Que *que) { return que->size; }

Effect *getFirstEffect(Que *que)
{
    if (isEmptyQue(que))
        return NULL;
    return que->head->effect;
}

void addLastEffectToQue(Que *queue, Effect *effect)
{
    EffectElement *new_e = malloc(sizeof(EffectElement));
    new_e->effect = effect;
    new_e->next = NULL;
    queue->tail->next = new_e;
    queue->tail = new_e;

    if (queue->size == 0)
        queue->head = new_e;

    queue->size++;
}

void destroyQue(Que *queue)
{
    EffectElement *cursor = queue->head;
    while (cursor != NULL)
    {
        EffectElement *next = cursor->next;
        if (cursor->effect)
            free(cursor->effect);
        free(cursor);
        cursor = next;
    }
    free(queue);
}

void printQue(Que *queue)
{

    if (queue->size == 0)
    {
        printf("[\n]\n");
        return;
    }

    EffectElement *cursor = queue->head;

    printf("[\n");
    for (int i = 0; i < queue->size; i++)
    {
        printf("#%d, cd: %d\n", i, cursor->effect->cooldown);
        cursor = cursor->next;
    }
    printf("]\n");
}

void *removeFirstEffect(Que *que)
{
    if (que->size == 0)
        return NULL;

    EffectElement *tmp = que->head;
    que->head = tmp->next;
    que->size--;
    free(tmp);
}

void *removeRowEffect(Que *queue, int y) {
    if (queue->head == NULL) return NULL;

    EffectElement *cursor = queue->head;
    EffectElement *prev = NULL;

    while (cursor != NULL) {
        if (cursor->effect->car->y == y) {
            if (prev == NULL) {
                queue->head = cursor->next;
            } else {
                prev->next = cursor->next;
            }

            free(cursor->effect);
            free(cursor);

            queue->size--;
        }

        prev = cursor;
        cursor = cursor->next;
    }
}
