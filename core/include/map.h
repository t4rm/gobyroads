#define GET(matrix, row, col) (matrix)->values[(row) * (matrix)->cols + (col)]
#define SET(matrix, row, col, value) ((matrix)->values[(row) * (matrix)->cols + (col)] = (value))

typedef struct grid
{
    int *values;
    int rows;
    int cols;
} Grid;

enum Occupation
{
    ROAD,
    USER,
    CAR,
    SAFE
    // TREE can be added if we want. It should imply that the zone beneath it is a safe zone though.
};

Grid initializeGrid(int rows, int columns);
void freeGrid(Grid *g);
void printGrid(Grid *g);
