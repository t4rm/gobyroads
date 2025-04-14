#include "gameState.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

void add_car(GameState *gameState){
    gameState->cars_amount+=1;
    Car new_car;
    new_car.size = 4;
    new_car.position_x = 1;
    new_car.position_y = 0;
    new_car.direction = rand()%2;
    if (new_car.direction == 0)
      new_car.direction = -1;
    gameState->cars[gameState->cars_amount-1]=new_car;
  }

  void update_cars(GameState *gameState){
    if(gameState->cars_cooldown==0){
      gameState->cars_cooldown=gameState->cars_cooldown;
      for(int i=0;i<gameState->cars_amount;i++)
        gameState->cars[i].position_x+=1*gameState->cars[i].direction;
    }
    else
      gameState->cars_cooldown-=1;
  }