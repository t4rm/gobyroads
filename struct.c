#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>  //pour créer de la rng
#include <ncurses.h>
#include <unistd.h>
#include "struct.h"


void init_game(){
  initscr();
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);
  srand(time(NULL));
  system ("/bin/stty raw");           //change le mode d'input
  srand(time(NULL));
}

void mapPrint(State gameState)
{
  int x, y;
  system("clear");
  printf("SCORE : %d\n\r",gameState.score);
  for(x=0; x<size_y; x++)
  {
    for(y=0; y<size_x; y++){
      int printed=false;
      if (x==gameState.player_y && y==gameState.player_x){
        printf("%c", '0');
        printed=true;
      }
      for (int i=0;i<gameState.cars_amount;i++){
        Car car=gameState.cars[i];
        if (x==car.position_y && y<=car.position_x && y>car.position_x-car.size && printed==false){
          if (car.direction==1)
            printf("%c",'>');
          else
            printf("%c",'<');
          printed=true;
        }
      }
      if(printed==false)
        printf("%c", gameState.map[x][y]);
    }
    printf("\n\r");
  }
  refresh();
}

void playerMove(State* p_gameState)
{
  int input;
    // Vérifier si une touche est pressée
    input = getch();
    if (input != ERR && p_gameState->player_mouv_cooldown==0) {
      switch(input)
      {
        case 'z': 
          if ((*p_gameState).player_y>=size_y-3)
            (*p_gameState).player_y--;
          else
            scrolling(p_gameState);
          p_gameState->player_mouv_cooldown=mouvement_cooldown;
          break;
        case 'q': 
          (*p_gameState).player_x--; 
          p_gameState->player_mouv_cooldown=mouvement_cooldown;
          break;
        case 's': 
          (*p_gameState).player_y++; 
          p_gameState->player_mouv_cooldown=mouvement_cooldown;
          break;
        case 'd': 
          (*p_gameState).player_x++; 
          p_gameState->player_mouv_cooldown=mouvement_cooldown;
          break;
        case 'f':
          p_gameState->player_x=0;    //pour tuer le joueur... a modifier
          break;
        case 'h':
          printf("Use ZQSD to move\n\rPress f to quit\n\r");
          break;
        default:
          printf("Use ZQSD to move !\n\r");
          break;
      }


    }
    if (p_gameState->player_mouv_cooldown>0)
      p_gameState->player_mouv_cooldown-=1;
}



State init_gameState() {
    State gameState={
      .cars_amount=0, 
      .cars_cooldown=0,
      .player_mouv_cooldown=0,
      .player_x=starting_position_x,
      .player_y=starting_position_y,
      .map={
      "############################",
      "#                          #",
      "#                          #",
      "#                          #",
      "#                          #",
      "#                          #",
      "#                          #",
      "#                          #",
      "#                          #",
      "#                          #",
      "#                          #",
      "############################"
      },
      .score=0
    };
    return(gameState);
  };

void scrolling(State* p_gameState){
  p_gameState->score++; 
  for (int i=size_y-2;i>1;i--){
    for (int j=1;j<size_x;j++){
      (*p_gameState).map[i][j]=(*p_gameState).map[i-1][j];
    }
  }
  for(int i=1;i<size_x-1;i++)
    p_gameState->map[1][i]=' ';
  add_car(p_gameState);
  for (int i=0;i<p_gameState->cars_amount;i++)
    p_gameState->cars[i].position_y+=1;
}

void update_cars(State *gameState){
  if(gameState->cars_cooldown==0){
    gameState->cars_cooldown=car_cooldown;
    for(int i=0;i<gameState->cars_amount;i++)
      gameState->cars[i].position_x+=1*gameState->cars[i].direction;
  }
  else
    gameState->cars_cooldown-=1;
}

bool check_collision(State *gameState){
  if(gameState->map[gameState->player_y][gameState->player_x]!=' '){
    printw("You died !\n");
    return true;  
  }
  else{
    for(int i=0;i<gameState->cars_amount;i++) {
      if (gameState->cars[i].position_y==gameState->player_y
      && gameState->player_x<=gameState->cars[i].position_x 
      && gameState->player_x>gameState->cars[i].position_x-gameState->cars[i].size
      ){
        printw("You died !\n");
        return true;
      }
    }
    return false;
  }
}

void add_car(State *gameState){
  gameState->cars_amount+=1;
  Car new_car;
  new_car.size = 4;
  new_car.position_x = 1+rand()%(size_x-2);
  new_car.position_y = 0;
  new_car.direction = rand()%2;
  if (new_car.direction == 0)
    new_car.direction = -1;
  gameState->cars[gameState->cars_amount-1]=new_car;
}