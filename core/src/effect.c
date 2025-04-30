#include "effect.h"
#include <stdlib.h>

void updateEffects(GameState *gs)
{
    EffectElement *cursor = gs->effects->head;

    if (!cursor)
        return;

    // Effect *effect = cursor->effect;

    if (cursor->effect && cursor->effect->car)
    {
        if (cursor->effect->car->y < 0)
        {
            removeFirstEffect(gs->effects);
            return;
        }

        if (cursor->effect->cooldown <= 0)
        {
            cursor->effect->function(gs->cars, cursor->effect->car);
            removeFirstEffect(gs->effects);
            return;
        }
    }
    else
    {
        while (cursor != NULL)
        {
            if (cursor->effect && cursor->effect->cooldown > 0)
                cursor->effect->cooldown--;
            cursor = cursor->next;
        }
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
