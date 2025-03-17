#include <stdio.h>
#include "map.h"

int main()
{
    Row *grid = createGrid(8, 10);

    displayGrid(grid,8);
    return 0;
}