#pragma once

typedef struct Node{
    int x;
    int y;
    int g;
    int h;
    int f;
    struct Node * parent;
}Node;

typedef struct PriorityQueue{
    Node **arr;
    int capacity;
    int size;
}PriorityQueue;


Node **aStar(GameState *gs, int xStart, int yStart, int xEnd, int yEnd, int *pathLength);
Node *createNode(int x, int y, int g, int h, Node *parent);
void freeNode(Node *node);
int calculateHeuristic(int xStart, int yStart, int xEnd, int yEnd);
void getNeighboursCoords(int x, int y, int maxX, int maxY, int list[4][2]);
bool coordInList(int x, int y, Node **list, int size);
bool isObstacle(GameState *gs, int nx, int ny, int g);
Node **reconstructPath(Node *end);

PriorityQueue *createPriorityQueue(int capacity);
void insertPriorityQueue(PriorityQueue *priorityQueue, Node *node);
Node *getAndRemoveMinFromPriorityQueue(PriorityQueue *priorityQueue);
void heapifyDown(PriorityQueue *priorityQueue, int i);
void heapifyUp(PriorityQueue *priorityQueue, int i);
Node *getFromPriorityQueue(PriorityQueue *priorityQueue, int x, int y);
int getIndexFromPriorityQueue(PriorityQueue *priorityQueue, Node *node);
void printPriorityQueue(PriorityQueue* priorityQueue);
void freePriorityQueue(PriorityQueue* priorityQueue);

