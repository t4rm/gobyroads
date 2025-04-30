#include <stdlib.h>
#include <stdio.h>
#include "effect_queue.h"
#include "gamestate.h"

EffectQueue *createEffectQueue(void)
{
    EffectQueue *new = malloc(sizeof(EffectQueue));
    new->size = 0;
    new->head = malloc(sizeof(EffectElement));
    new->tail = malloc(sizeof(EffectElement));
    return new;
}

void addLastEffectToEffectQueue(EffectQueue *queue, Effect *effect)
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

void destroyEffectQueue(EffectQueue *queue)
{
    EffectElement *cursor = queue->head;
    while (cursor != NULL)
    {
        EffectElement *next = cursor->next;
        if (cursor->effect) {
            if(cursor->effect->car) {
                    free(cursor->effect->car);
                }
                free(cursor->effect);
            }
        free(cursor);
        cursor = next;
    }
    free(queue);
}

void destroyEffect(EffectElement *element)
{
    free(element->effect->car);
    free(element->effect);
    free(element);
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

void removeFirstEffect(EffectQueue *que)
{
    if (que->size == 0)
        return;

    EffectElement *tmp = que->head;
    que->head = tmp->next;
    que->size--;
    free(tmp);
}

void removeRowEffect(EffectQueue *queue, int y) {
    if (queue->head == NULL) return;

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
