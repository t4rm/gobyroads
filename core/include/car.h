#ifndef CAR_H
#define CAR_H

typedef struct _GameState GameState;

typedef struct _Car
{
    int position_y, position_x;
    int size;
    int direction;
} Car;

void add_car(struct _GameState *gameState);
void update_cars(struct _GameState *gameState);

#endif