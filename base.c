#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>  //pour créer de la rng

#define size_x 28
#define size_y 12
#define starting_position_x 13
#define starting_position_y 8

typedef struct _state {                    //contiend toutes les infos sur l'etat du jeu
  int player_x, player_y;
  char map[size_y][size_x];
  int score;
}State;

State init_gameState();               //declarations des fonctions
void mapPrint(State gameState);
bool playerMove(State* p_gameState);
void scroll(State* gameState);

int main()
{
  srand(time(NULL));
  bool collision=false;
  State gameState=init_gameState();
  system ("/bin/stty raw");           //change le mode d'input

  while(collision==false){
    update_game(gameState);
    mapPrint(gameState);
    collision=playerMove(&gameState); 
  }
  system ("/bin/stty cooked");

  return 0;
}

void update_game(){
  
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
        printf("%c", 'X');
        printed=true;
      }
      if (i==gameState.player_y-1 && j==gameState.player_x){
        printf("%c", '0');
        printed=true;
      }
      if(printed==false)
        printf("%c", gameState.map[i][j]);
    }
    printf("\n\r");
  }
}

bool playerMove(State* p_gameState)
{
  char input=getchar();

  if (input!='z' && input!='q' && input!='s' && input!='d' && input!='h')
  {
    printf("Use ZQSD to move !\n");
    input=getchar();
  }
  if (input=='h'){
    printf("<put help here>\n");
    input=getchar();
  }

  switch(input)
  {
    case 'z': 
      if ((*p_gameState).player_y>=size_y-3)
        (*p_gameState).player_y--;
      else
        scroll(p_gameState);
      break;
    case 'q': (*p_gameState).player_x--; break;
    case 's': (*p_gameState).player_y++; break;
    case 'd': (*p_gameState).player_x++; break;
  }

  if((*p_gameState).map[(*p_gameState).player_y][(*p_gameState).player_x]==' ')   //check collision
    return false;
  printf("You died !\n\r");
  return true;  
}

State init_gameState() {
    State gameState={
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

void scroll(State* p_gameState){
  (*p_gameState).score++; 
  for (int i=size_y-2;i>1;i--){
    for (int j=1;j<size_x;j++){
      (*p_gameState).map[i][j]=(*p_gameState).map[i-1][j];
    }
  }
  for(int i=1;i<size_x-1;i++)
    (*p_gameState).map[1][i]=' ';
  (*p_gameState).map[1][1+rand()%(size_x-2)]='V';
}