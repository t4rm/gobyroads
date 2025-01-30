#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>  //pour créer de la rng

#define size_x 28
#define size_y 12
#define starting_position_x 13
#define starting_position_y 8

typedef struct {                    //condiend toutes les infos sur l'etat du jeu
  int player_x, player_y;
  char map[size_y][size_x];
  int score;
}_state;

_state init_gameState();               //declarations des fonctions
void mapPrint(_state gameState);
bool playerMove(_state* p_gameState);
void scroll(_state* gameState);

int main()
{
  srand(time(NULL));
  bool collision=false;
  _state gameState=init_gameState();
  system ("/bin/stty raw");           //change le mode d'input
  while(collision==false){
    mapPrint(gameState);
    collision=playerMove(&gameState); 
  }
  system ("/bin/stty cooked");

  return 0;
}

void mapPrint(_state gameState)
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

bool playerMove(_state* p_gameState)
{
  char c=getchar();

  if (c!='z' && c!='q' && c!='s' && c!='d' && c!='h')
  {
    printf("Use ZQSD to move !\n");
    c=getchar();
  }
  if (c=='h'){
    printf("<put help here>\n");
    c=getchar();
  }

  switch(c)
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

_state init_gameState() {
    _state gameState={
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

void scroll(_state* p_gameState){
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