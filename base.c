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
#define car_cooldown 10

typedef struct _car {     
  int position_y, position_x;
  int size;
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
        if (x==gameState.cars[i].position_y && y<=gameState.cars[i].position_x && y>gameState.cars[i].position_x-gameState.cars[i].size && printed==false){
          printf("%c",'>');
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
          printw("Press f to quit\n");
          break;
        default:
          printw("Use ZQSD to move !\n");
          break;
      }


    }
    if (p_gameState->player_mouv_cooldown>0)
      p_gameState->player_mouv_cooldown-=1;
}

void init_game(){
  initscr();
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);
  srand(time(NULL));
  system ("/bin/stty raw");           //change le mode d'input
  srand(time(NULL));
}

State init_gameState() {
    State gameState={
      .cars_amount=0, 
      .cars_cooldown=0,
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
  p_gameState->score++; 
  for (int i=size_y-2;i>1;i--){
    for (int j=1;j<size_x;j++){
      (*p_gameState).map[i][j]=(*p_gameState).map[i-1][j];
    }
  }
  for(int i=1;i<size_x-1;i++)
    p_gameState->map[1][i]=' ';
  p_gameState->cars_amount+=1;
  p_gameState->cars[p_gameState->cars_amount-1].size =5;
  p_gameState->cars[p_gameState->cars_amount-1].position_x=1+rand()%(size_x-2);
  p_gameState->cars[p_gameState->cars_amount-1].position_y=0;
  for (int i=0;i<p_gameState->cars_amount;i++)
    p_gameState->cars[i].position_y+=1;
}

void update_cars(State *gameState){
  if(gameState->cars_cooldown==0){
    gameState->cars_cooldown=car_cooldown;
    for(int i=0;i<gameState->cars_amount;i++)
      gameState->cars[i].position_x+=1;
  }
  else
    gameState->cars_cooldown-=1;
}

bool check_collision(State *gameState){
  if(gameState->map[gameState->player_y][gameState->player_x]!=' '){
    printf("You died !\n\r");
    return true;  
  }
  else{
    for(int i=0;i<gameState->cars_amount;i++) {
      if (gameState->cars[i].position_y==gameState->player_y
      && gameState->player_x<=gameState->cars[i].position_x 
      && gameState->player_x>gameState->cars[i].position_x-gameState->cars[i].size
      ){
        printf("You died !\n\r");
        return true;
      }
    }
    return false;
  }
}