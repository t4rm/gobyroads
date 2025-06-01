#include "gamestate.h"
#include "a_star.h"
#include <stdlib.h>
#include <stdbool.h>

Node *createNode(int x, int y, int g, int h, Node *parent)
{
    Node *node = (Node *)malloc(sizeof(Node));

    node->x = x;
    node->y = y;
    node->g = g;
    node->h = h;
    node->f = g + h;
    node->parent = parent;

    return node;
}

void freeNode(Node *node)
{
    free(node);
}

Node **aStar(GameState *gs, int xStart, int yStart, int xEnd, int yEnd, int *routeLength)
{

    // on intialise le noeud de départ avec g = 0; h = heuristique
    int heuristicStart = calculateHeuristic(xStart, yStart, xEnd, yEnd);
    Node *start = createNode(xStart, yStart, 0, heuristicStart, NULL);
    Node *current = NULL;

    // création des openLists et closedLists de taille nb de cases de la grille
    PriorityQueue *openList = createPriorityQueue(gs->grid->height * gs->grid->length);
    Node **closedList = (Node **)malloc(sizeof(Node *) * gs->grid->height * gs->grid->length);
    int closedListSize = 0;

    int neighbours[4][2];

    insertPriorityQueue(openList, start);

    while (openList->size > 0)
    {

        // le noeud courant devient le noeud avec la plus petite valeur de f
        current = getAndRemoveMinFromPriorityQueue(openList);

        if (current->x == xEnd && current->y == yEnd)
        {

            *routeLength = current->g + 1;

            free(closedList);
            return reconstructPath(current);
        }

        // on ajoute le noeud courant a la closedList
        closedList[closedListSize] = current;
        closedListSize++;

        // on récupère les noeuds voisins (droite, gauche, bas, haut)
        getNeighboursCoords(current->x, current->y, (gs->grid->length + 2 * gs->carMaxSize) / 2, gs->grid->height, neighbours);

        for (int i = 0; i < 4; i++)
        {

            int nx = neighbours[i][0];
            int ny = neighbours[i][1];

            // on vérifie que le voisin n'est pas en dehors de la grille
            if (nx != -1 && ny != -1)
            {

                // on verifie qu'il n'a pas dejà été exploré
                if (coordInList(nx, ny, closedList, closedListSize) == false)
                {

                    // on vérifie que ce n'est pas un obstacle
                    if (isObstacle(gs, nx, ny, current->g+1) == false)
                    {

                        int g = current->g + 1;
                        int h = calculateHeuristic(nx, ny, xEnd, yEnd);
                        Node *neighbour = getFromPriorityQueue(openList, nx, ny);

                        // si le voisin n'est pas dans l'openlist alors on l'ajuste
                        if (neighbour == NULL)
                        {
                            neighbour = createNode(nx, ny, g, h, current);
                            insertPriorityQueue(openList, neighbour);
                        }
                        // sinon si son g est réduit alors on l'actualise
                        else if (g < neighbour->g)
                        {
                            neighbour->g = g;
                            neighbour->f = h + g;
                            neighbour->parent = current;
                            heapifyUp(openList, getIndexFromPriorityQueue(openList, neighbour));
                        }
                    }
                }
            }
        }
    }
    *routeLength = 0;

    freePriorityQueue(openList);
    free(closedList);
    return NULL;
}

// calcul de l'heuristique avec la distance de Manhattan
int calculateHeuristic(int xStart, int yStart, int xEnd, int yEnd)
{
    return abs(xStart - xEnd) + abs(yStart - yEnd);
}

bool coordInList(int x, int y, Node **list, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (list[i]->x == x && list[i]->y == y)
        {
            return true;
        }
    }
    return false;
}

bool isObstacle(GameState *gs, int nx, int ny, int g)
{
    Occupation cell = gs->grid->cases[ny][nx];

    if (cell == WATER || cell == TREE || cell == CAR_LEFT || cell == CAR_RIGHT)
    {
        return true;
    }
    if(cell == ROAD){
       // int speed = gs->grid->
    }
    //  printf("Safe\n");
    return false;
}

// renvoie un tableau de coordonées des voisins de res et -1 -1 si en dehors de la grille
void getNeighboursCoords(int x, int y, int maxX, int maxY, int list[4][2])
{

    if (x + 1 < maxX)
    {
        list[0][0] = x + 1;
        list[0][1] = y;
    }
    else
    {
        list[0][0] = -1;
        list[0][1] = -1;
    }

    if (x - 1 >= 0)
    {
        list[1][0] = x - 1;
        list[1][1] = y;
    }
    else
    {
        list[1][0] = -1;
        list[1][1] = -1;
    }

    if (y + 1 < maxY)
    {
        list[2][0] = x;
        list[2][1] = y + 1;
    }
    else
    {
        list[2][0] = -1;
        list[2][1] = -1;
    }

    if (y - 1 >= 0)
    {
        list[3][0] = x;
        list[3][1] = y - 1;
    }
    else
    {
        list[3][0] = -1;
        list[3][1] = -1;
    }
}

Node **reconstructPath(Node *end)
{
    int length = end->g + 1;
    Node **path = (Node **)malloc(sizeof(Node *) * length);

    Node *current = end;
    for (int i = length - 1; i >= 0; i--)
    {
        path[i] = current;
        current = current->parent;
    }
  
   
    // for (int i = 0; i < length; i++){
    //     printf("(%d, %d) ", path[i]->x, path[i]->y);
    // }

    return path;
}

PriorityQueue *createPriorityQueue(int capacity)
{
    PriorityQueue *priorityQueue = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    priorityQueue->capacity = capacity;
    priorityQueue->size = 0;
    priorityQueue->arr = (Node **)malloc(sizeof(Node) * capacity);

    return priorityQueue;
}

void insertPriorityQueue(PriorityQueue *priorityQueue, Node *node)
{
    priorityQueue->size++;
    int i = priorityQueue->size - 1;
    priorityQueue->arr[i] = node;

    while (i != 0 && priorityQueue->arr[(i - 1) / 2]->h > priorityQueue->arr[i]->h)
    {
        Node *temp = priorityQueue->arr[i];
        priorityQueue->arr[i] = priorityQueue->arr[(i - 1) / 2];
        priorityQueue->arr[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}

Node *getAndRemoveMinFromPriorityQueue(PriorityQueue *priorityQueue)
{
    Node *res = priorityQueue->arr[0];

    priorityQueue->arr[0] = priorityQueue->arr[priorityQueue->size - 1];
    priorityQueue->size--;
    heapifyDown(priorityQueue, 0);

    return res;
}

Node *getFromPriorityQueue(PriorityQueue *priorityQueue, int x, int y)
{
    for (int i = 0; i < priorityQueue->size; i++)
    {
        if (priorityQueue->arr[i]->x == x && priorityQueue->arr[i]->y == y)
        {
            return priorityQueue->arr[i];
        }
    }
    return NULL;
}

int getIndexFromPriorityQueue(PriorityQueue *priorityQueue, Node *node)
{
    for (int i = 0; i < priorityQueue->size; i++)
    {
        if (priorityQueue->arr[i] == node)
        {
            return i;
        }
    }
    return -1;
}

void heapifyDown(PriorityQueue *priorityQueue, int i)
{
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < priorityQueue->size && priorityQueue->arr[left]->f < priorityQueue->arr[smallest]->f)
    {
        smallest = left;
    }

    if (right < priorityQueue->size && priorityQueue->arr[right]->f < priorityQueue->arr[smallest]->f)
    {
        smallest = right;
    }

    if (smallest != i)
    {
        Node *temp = priorityQueue->arr[i];
        priorityQueue->arr[i] = priorityQueue->arr[smallest];
        priorityQueue->arr[smallest] = temp;

        heapifyDown(priorityQueue, smallest);
    }
}

void heapifyUp(PriorityQueue *priorityQueue, int i)
{
    while (i > 0)
    {
        int parent = (i - 1) / 2;
        if (priorityQueue->arr[i]->f < priorityQueue->arr[parent]->f)
        {
            Node *temp = priorityQueue->arr[i];
            priorityQueue->arr[i] = priorityQueue->arr[parent];
            priorityQueue->arr[parent] = temp;
            i = parent;
        }
        else
        {
            break;
        }
    }
}

void printPriorityQueue(PriorityQueue *priorityQueue)
{
    for (int i = 0; i < priorityQueue->size; i++)
    {
        printf("[%d] ", priorityQueue->arr[i]->f);
    }
    printf("\n");
}

void freePriorityQueue(PriorityQueue *priorityQueue)
{
    for (int i = 0; i < priorityQueue->size; i++)
    {
        freeNode(priorityQueue->arr[i]);
    }
    free(priorityQueue->arr);
    free(priorityQueue);
}