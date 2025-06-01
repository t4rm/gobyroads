#include "gamestate.h"
#include "a_star.h"
#include <stdlib.h>
#include <stdbool.h>

/* Create a node
 * x, y : position of the node on the grid
 * g  : cost from the start node to this node
 * h : heuristic cost
 * parent: pointer to the parent node
 *
 * returns a pointer to node created
 */
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

/* Free the node passed in parameter
 */
void freeNode(Node *node)
{
    free(node);
}

/* Search a path between two points using A* algorithm
 * gs : the gamestate
 * xStart, yStart : position of the start node on the grid
 * xEnd, yEnd : position of the end node on the grid
 * pathLength : pointer used to get the length of the path
 *
 * returns the path as a list of Nodes or NULL if no path exists
 */
Node **aStar(GameState *gs, int xStart, int yStart, int xEnd, int yEnd, int *pathLength)
{
    // initialization of the start node with g = 0 and heuristic to the end
    int heuristicStart = calculateHeuristic(xStart, yStart, xEnd, yEnd);
    Node *start = createNode(xStart, yStart, 0, heuristicStart, NULL);

    // initialization of the current node
    Node *current = NULL;

    // creation of the openList and closedList with a maxSize = number of cells in the grid
    PriorityQueue *openList = createPriorityQueue(gs->grid->height * gs->grid->length);
    Node **closedList = (Node **)malloc(sizeof(Node *) * gs->grid->height * gs->grid->length);
    int closedListSize = 0;

    // initialization of the list that will store the coordinates of the neighbours of the current node
    int neighbours[4][2];

    // insertion of the start node in the openList
    insertPriorityQueue(openList, start);

    //  while there are still nodes to explore in the openList
    while (openList->size > 0)
    {
        // we take the node with the lowest f value and remove it from the priority queue
        current = getAndRemoveMinFromPriorityQueue(openList);

        // if the node is our goal
        if (current->x == xEnd && current->y == yEnd)
        {
            // the pathLength = number of nodes in the path = g + 1
            *pathLength = current->g + 1;
            free(closedList);
            // reconstruct and return the path
            return reconstructPath(current);
        }

        // else add current node to the closedList
        closedList[closedListSize] = current;
        closedListSize++;

        // get the neighbours of the current node
        getNeighboursCoords(current->x, current->y, (gs->grid->length + 2 * gs->carMaxSize) / 2, gs->grid->height, neighbours);

        // for each neighbour
        for (int i = 0; i < 4; i++)
        {
            int nx = neighbours[i][0];
            int ny = neighbours[i][1];

            // if it is not outside the grid
            if (nx != -1 && ny != -1)
            {

                // if it has not already been explored (not in closedList)
                if (coordInList(nx, ny, closedList, closedListSize) == false)
                {

                    // if it is not an obstacle
                    if (isObstacle(gs, nx, ny, current->g + 1) == false)
                    {
                        // we calculate the neighbour's h and g values
                        int g = current->g + 1;
                        int h = calculateHeuristic(nx, ny, xEnd, yEnd);

                        // if the neighbour is not in the openList, we add it
                        Node *neighbour = getFromPriorityQueue(openList, nx, ny);
                        if (neighbour == NULL)
                        {
                            neighbour = createNode(nx, ny, g, h, current);
                            insertPriorityQueue(openList, neighbour);
                        }
                        // else, if his g is lower than it was, we actualise it
                        else if (g < neighbour->g)
                        {
                            neighbour->g = g;
                            neighbour->f = h + g;
                            neighbour->parent = current;
                            // actualisation of the priorityqueue
                            heapifyUp(openList, getIndexFromPriorityQueue(openList, neighbour));
                        }
                    }
                }
            }
        }
    }

    // if no path is found
    *pathLength = 0;
    freePriorityQueue(openList);
    free(closedList);
    return NULL;
}

/* Calculate the heuristic between to points using Manhattan distance
 * xStart, yStart : position of the start node on the grid
 * xEnd, yEnd : position of the end node on the grid
 * returns integer heuristic value
 */
int calculateHeuristic(int xStart, int yStart, int xEnd, int yEnd)
{
    return abs(xStart - xEnd) + abs(yStart - yEnd);
}

/* Check if coordinates are present inside a nodes list
 * x, y : coordinates
 * **list : nodes list
 * size : size of the nodes list
 * returns true if present, else false
 */
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

/* Check if coordinates are an obstacle
 * gs : teh gameState
 * nx, ny : coordinates
 * g : the number of movements before arriving on the cell (used to predict car movements)
 * returns true if obstacle, else false
 */
bool isObstacle(GameState *gs, int nx, int ny, int g)
{

    Occupation cell = gs->grid->cases[ny][nx];

    if (cell == WATER || cell == TREE || cell == CAR_LEFT || cell == CAR_RIGHT)
    {
        return true;
    }
    if (cell == ROAD && g != 0)
    {
        // speed of the car (number of frames needed to go to the next cell)
        int speed = gs->grid->rowManagers[ny]->speed;

        // number of frames before the player go to cell, g = numbers of cells to cross * 8 = number of frame by movement
        int framesBeforeArriving = (g  ) * 10;

        // number of cells crossed by the car
        int nbCellPredicted = framesBeforeArriving / speed;

        // check if the cell would be occupied by a car at nbCellPredicted distance (we add a range of 2 for better results, probably due to desynchronisation of the frames, thing to upgrade)
        if (gs->grid->cases[ny][nx - nbCellPredicted] == CAR_RIGHT || gs->grid->cases[ny][nx + nbCellPredicted] == CAR_LEFT || gs->grid->cases[ny][nx - nbCellPredicted + 1] == CAR_RIGHT || gs->grid->cases[ny][nx + nbCellPredicted - 1] == CAR_LEFT ||  gs->grid->cases[ny][nx - nbCellPredicted - 1] == CAR_RIGHT || gs->grid->cases[ny][nx + nbCellPredicted + 1] == CAR_LEFT)
        {
            return true;
        }
    }
    return false;
}

/* Get the list of the neighbours coords of give coords
 * x, y : coordinates
 * maxX, maxY : max coordinates that belongs to the grid
 * list : list to fill with the coordinates
 */
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

/* Reconstruct the path from the start to the end
 * end : pointer to the end node
 * returns a pointer to a list of nodes containing the path from the start to the end
 */
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

    return path;
}

/* Create a priority queue of nodes
 * capacity : capacity of the priority queue
 * returns a pointer to the priority queue created
 */
PriorityQueue *createPriorityQueue(int capacity)
{
    PriorityQueue *priorityQueue = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    priorityQueue->capacity = capacity;
    priorityQueue->size = 0;
    priorityQueue->arr = (Node **)malloc(sizeof(Node) * capacity);

    return priorityQueue;
}

/* Insert a node in a priority queue
 * priorityQueue : pointer to the priority queue
 * node : pointer to the node to be ionserted
 */
void insertPriorityQueue(PriorityQueue *priorityQueue, Node *node)
{
    priorityQueue->size++;
    int i = priorityQueue->size - 1;
    priorityQueue->arr[i] = node;

    while (i != 0 && priorityQueue->arr[(i - 1) / 2]->f > priorityQueue->arr[i]->f)
    {
        Node *temp = priorityQueue->arr[i];
        priorityQueue->arr[i] = priorityQueue->arr[(i - 1) / 2];
        priorityQueue->arr[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}

/* Get the minimum value of the priority queue and withdraw it
 * priorityQueue : pointer to the priority queue
 * node : pointer to the node with the min priority
 */
Node *getAndRemoveMinFromPriorityQueue(PriorityQueue *priorityQueue)
{
    Node *res = priorityQueue->arr[0];

    priorityQueue->arr[0] = priorityQueue->arr[priorityQueue->size - 1];
    priorityQueue->size--;
    heapifyDown(priorityQueue, 0);

    return res;
}

/* Get a node from the priority queue given its coords
 * priorityQueue : pointer to the priority queue
 * x,y coordinates of the node
 * return the nodes that correspond to these coords or NULL if not found
 */
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

/* Get the index of a node from the priority queue
 * priorityQueue : pointer to the priority queue
 * node : pointer to the node
 * return the index of the node if found, else -1
 */
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

/* Actualise the heap by moving the element at the index i down in the heap
 * priorityQueue : pointer to the priority queue
 * i, index of the node to move
 */
void heapifyDown(PriorityQueue *priorityQueue, int i)
{
    int smallest = i;

    // index of the left child
    int left = 2 * i + 1;
    // index of the right child
    int right = 2 * i + 2;

    // if the f value of the left child is lower than the smallest, it becomes the smallest
    if (left < priorityQueue->size && priorityQueue->arr[left]->f < priorityQueue->arr[smallest]->f)
    {
        smallest = left;
    }

    // same for the right child
    if (right < priorityQueue->size && priorityQueue->arr[right]->f < priorityQueue->arr[smallest]->f)
    {
        smallest = right;
    }

    // if i was not the smallest we swap i and the smallest
    if (smallest != i)
    {
        Node *temp = priorityQueue->arr[i];
        priorityQueue->arr[i] = priorityQueue->arr[smallest];
        priorityQueue->arr[smallest] = temp;

        // we continue with the children of smallest
        heapifyDown(priorityQueue, smallest);
    }
}

/* Actualise the heap by moving the element at index i up 
 * priorityQueue : pointer to the priority queue
 * i : index of the node to move up
 */
void heapifyUp(PriorityQueue *priorityQueue, int i)
{
    // while the node is not at the root
    while (i > 0)
    {
        // index of the parent node
        int parent = (i - 1) / 2;

        // if the f value of the current node is less than its parent's
        if (priorityQueue->arr[i]->f < priorityQueue->arr[parent]->f)
        {
            // swap the current node with its parent
            Node *temp = priorityQueue->arr[i];
            priorityQueue->arr[i] = priorityQueue->arr[parent];
            priorityQueue->arr[parent] = temp;

            // continue with the parent
            i = parent;
        }
        else
        {
            break;
        }
    }
}

/* Print the priority queue (used for tests)
 * priorityQueue : pointer to the priority queue
 */
void printPriorityQueue(PriorityQueue *priorityQueue)
{
    for (int i = 0; i < priorityQueue->size; i++)
    {
        printf("[%d] ", priorityQueue->arr[i]->f);
    }
    printf("\n");
}


/* Free the priority queue
 * priorityQueue : pointer to the priority queue
 */
void freePriorityQueue(PriorityQueue *priorityQueue)
{
    for (int i = 0; i < priorityQueue->size; i++)
    {
        freeNode(priorityQueue->arr[i]);
    }
    free(priorityQueue->arr);
    free(priorityQueue);
}