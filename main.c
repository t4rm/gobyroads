#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>  //pour créer de la rng
#include <ncurses.h>
#include <unistd.h>
#include "struct.h"


int main()
{
  init_game();

  bool collision=false;
  State gameState=init_gameState();

  system ("/bin/stty raw");           //change le mode d'input
  while(collision==false){
    mapPrint(gameState);
    playerMove(&gameState); 
    collision=check_collision(&gameState);
    update_cars(&gameState);
    collision=check_collision(&gameState);
    usleep(16667);                  // on attend environ 16 ms pour obtenir 60 FPS
  }
  system ("/bin/stty cooked");

  endwin();
  return 0;
}