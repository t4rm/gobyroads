#ifndef EFFECT_H
#define EFFECT_H

#include "gamestate.h"

typedef struct _GameState GameState;

void updateEffects(GameState *gs);
void decrementEffectsOnY(GameState *gs);

#endif
