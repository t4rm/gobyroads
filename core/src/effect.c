#include "effect.h"
#include <stdlib.h>

void updateEffects(GameState *gs)
{
    EffectElement *cursor = gs->effects->head;
    EffectElement *previous = NULL;

    while (cursor != NULL)
    {
        EffectElement *next = cursor->next;
        Effect *e = cursor->effect;

        bool shouldRemove = false;

        if (e && e->car && e->car->y < 0)
        {
            // The effect is no longer relevant (off-screen)
            shouldRemove = true;
        }
        else if (e && e->cooldown == 0) // TODO : Factorize, maybe use removeRow and put y to -1.
        {
            // Execute the effect
            e->function(gs->cars, e->car);
            shouldRemove = true;
        }
        else if (e)
        {
            e->cooldown--;
        }

        // if (shouldRemove)
        // {
        //     if (previous == NULL)
        //         gs->effects->head = next;
        //     else
        //         previous->next = next;

        //     if (cursor == gs->effects->tail)
        //         gs->effects->tail = previous;

        //     if (e) {
        //         if (e->car) free(e->car);
        //         free(e);
        //     }
        //     free(cursor);
        //     gs->effects->size--;
        // }
        // else
        // {
        //     previous = cursor;
        // }

        cursor = next;
    }
}


void decrementEffectsOnY(GameState *gs)
{
    EffectElement *cursor = gs->effects->head;
    EffectElement *prev = NULL;

    while (cursor != NULL)
    {
        Effect *e = cursor->effect;
        if (e && e->car)
            e->car->y--;

        if (e && e->car && e->car->y < 0)
        {
            EffectElement *toDelete = cursor;
            if (prev == NULL)
                gs->effects->head = cursor->next;
            else
                prev->next = cursor->next;

            cursor = cursor->next;

            if (toDelete->effect)
            {
                if (toDelete->effect->car)
                    free(toDelete->effect->car);
                free(toDelete->effect);
            }
            free(toDelete);
            gs->effects->size--;

            continue;
        }

        prev = cursor;
        cursor = cursor->next;
    }
}
