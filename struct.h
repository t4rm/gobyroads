#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>  //pour créer de la rng
#include <ncurses.h>
#include <unistd.h>

#ifndef STRUCT
#define STRUCT

#define size_x 28
#define size_y 12
#define starting_position_x 13
#define starting_position_y 8
#define mouvement_cooldown 5
#define car_cooldown 10

typedef struct _car {     
    int position_y, position_x;
    int size;
    int direction;
  }Car;
  
  typedef struct _state {                    //contiend toutes les infos sur l'etat du jeu
    int player_x, player_y;
    int player_mouv_cooldown;
    int cars_amount;
    int cars_cooldown;
    Car cars[size_x*size_y];
    char map[size_y][size_x];
    int score;
  }State;
  
  State init_gameState();            //declarations des fonctions
  void init_game();
  void mapPrint(State gameState);
  void playerMove(State* p_gameState);
  void scrolling(State* gameState);
  void update_cars(State *gameState);
  bool check_collision(State *gameState);
  void add_car(State *gameState);

#endif