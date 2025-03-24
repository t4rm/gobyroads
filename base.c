#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>  //pour créer de la rng
#include <ncurses.h>
#include <unistd.h>

#define size_x 28
#define size_y 12
#define starting_position_x 13
#define starting_position_y 8
#define mouvement_cooldown 10
#define mouvement_cooldown 10

typedef struct _state {                    //contiend toutes les infos sur l'etat du jeu
  int player_x, player_y;
  int player_mouv_cooldown;
  char map[size_y][size_x];
  int score;
}State;

State init_gameState();            //declarations des fonctions
void init_game();
void mapPrint(State gameState);
bool playerMove(State* p_gameState);
void scrolling(State* gameState);

int main()
{
  init_game();
  initscr();
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);

  srand(time(NULL));
  bool collision=false;
  State gameState=init_gameState();

  system ("/bin/stty raw");           //change le mode d'input
  while(collision==false){
    mapPrint(gameState);
    collision=playerMove(&gameState); 
    // Attendre environ 16 ms pour obtenir 60 FPS
    usleep(16667);
  }
  system ("/bin/stty cooked");

  endwin();
  return 0;
}

void mapPrint(State gameState)
{
  int i, j;
  system("clear");
  printf("SCORE : %d\n\r",gameState.score);
  for(i=0; i<size_y; i++)
  {
    for(j=0; j<size_x; j++){
      int printed=false;
      if (i==gameState.player_y && j==gameState.player_x){
        printf("%c", '0');
        printed=true;
      }
      if(printed==false)
        printf("%c", gameState.map[i][j]);
    }
    printf("\n\r");
  }
  refresh();
}

bool playerMove(State* p_gameState)
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
          return true;
          break;
        case 'h':
          printf("Press f to quit\n");
          break;
        default:
          printf("Use ZQSD to move !\n");
          break;
      }


    }
    if (p_gameState->player_mouv_cooldown>0)
      p_gameState->player_mouv_cooldown-=1;

  if((*p_gameState).map[(*p_gameState).player_y][(*p_gameState).player_x]==' ')   //check collision (should be a separate function ?)
    return false;
  printf("You died !\n\r");
  return true;  
}

void init_game(){
  initscr();
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);
  srand(time(NULL));
  system ("/bin/stty raw");           //change le mode d'input
}

State init_gameState() {
    State gameState={
      .player_mouv_cooldown=0,
      .player_x=starting_position_x,
      .player_y=starting_position_y,
      .map={
      "############################",           //transformer en boucles ?
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
  (*p_gameState).score++; 
  for (int i=size_y-2;i>1;i--){
    for (int j=1;j<size_x;j++){
      (*p_gameState).map[i][j]=(*p_gameState).map[i-1][j];
    }
  }
  for(int i=1;i<size_x-1;i++)
    (*p_gameState).map[1][i]=' ';
  (*p_gameState).map[1][1+rand()%(size_x-2)]='>';
}