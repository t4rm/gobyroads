#include <stdlib.h>
#include <stdio.h>
#include "effect_queue.h"

EffectQueue *createEffectQueue(void)
{
    EffectQueue *new = malloc(sizeof(EffectQueue));
    new->size = 0;
    new->head = NULL;
    new->tail = NULL;
    return new;
}

void destroyEffectQueue(EffectQueue *queue)
{
    EffectElement *cursor = queue->head;
    while (cursor != NULL)
    {
        EffectElement *next = cursor->next;
        if (cursor->effect)
        {
            if (cursor->effect->car)
                free(cursor->effect->car);
            free(cursor->effect);
        }
        free(cursor);
        cursor = next;
    }
    free(queue);
}

void removeFirstEffect(EffectQueue *queue)
{
    if (queue->size == 0)
        return;

    EffectElement *tmp = queue->head;
    queue->head = tmp->next;
    if (tmp->effect)
    {
        if (tmp->effect->car)
            free(tmp->effect->car);
        free(tmp->effect);
    }
    free(tmp);
    queue->size--;

    if (queue->size == 0)
        queue->tail = NULL;
}

void removeRowEffect(EffectQueue *queue, int y)
{
    if (queue->head == NULL)
        return;

    EffectElement *cursor = queue->head;
    EffectElement *prev = NULL;

    while (cursor != NULL)
    {
        EffectElement *next = cursor->next;

        if (cursor->effect && cursor->effect->car && cursor->effect->car->y == y)
        {
            if (prev == NULL)
                queue->head = next;
            else
                prev->next = next;

            if (cursor == queue->tail)
                queue->tail = prev;

            printf("\nrow %d deleted\n", cursor->effect->car->y);
            free(cursor->effect->car);
            free(cursor->effect);
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

void addLastEffect(EffectQueue *queue, Effect *effect)
{
    EffectElement *new_e = malloc(sizeof(EffectElement));
    new_e->effect = effect;
    new_e->next = NULL;

    if (queue->tail)
        queue->tail->next = new_e;
    else
        queue->head = new_e;

    queue->tail = new_e;
    queue->size++;
}

void printEffectQueue(EffectQueue *queue)
{

    if (queue->size == 0)
    {
        printf("\n[]\n");
        return;
    }

    EffectElement *cursor = queue->head;

    for (int i = 0; i < queue->size; i++)
    {
        printf("- %d, cd: %d\n", i, cursor->effect->cooldown);
        cursor = cursor->next;
    }
}
