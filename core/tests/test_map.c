#include "unity.h"
#include "map.h"
#include <stdlib.h>

Occupation array[] = {ROAD, WATER, RAIL};
int lenArr = sizeof(array) / sizeof(Occupation); // Or hardcoded 3.

void testCreateRowManager(void)
{
    for (int i = 0; i < lenArr; i++)
    {
        RowManager *rm = createRowManager(1, 25, array[i]);

        TEST_ASSERT_NOT_NULL(rm);
        TEST_ASSERT_EQUAL(1, rm->direction);
        TEST_ASSERT_EQUAL(25, rm->speed);
        if (array[i] != RAIL)
            TEST_ASSERT_EQUAL(rm->speed, rm->cooldown); // Speed should equals cooldown when creating a rowManager unless it's a RAIL.
        TEST_ASSERT_EQUAL(array[i], rm->type);

        free(rm);
    }
}

void testCreateRow(void)
{
    for (int i = 0; i < lenArr; i++)
    {
        int l = 10;
        Occupation *row = createRow(l, array[i]);

        TEST_ASSERT_NOT_NULL(row);

        for (int j = 0; j < l; j++)
            TEST_ASSERT_EQUAL(array[i], row[j]);

        free(row);
    }
}

void testCreateSafeRowWithTrees(void)
{
    int l = 15;
    Occupation *row = createRow(l, SAFE);

    TEST_ASSERT_NOT_NULL(row);

    int treeCount = 0;
    int safeCount = 0;

    for (int i = 0; i < l; i++)
        if (row[i] == TREE)
            treeCount++;
        else if (row[i] == SAFE)
            safeCount++;

    TEST_ASSERT_GREATER_THAN(0, treeCount);
    TEST_ASSERT_GREATER_THAN(0, safeCount);
    TEST_ASSERT_EQUAL(l, treeCount + safeCount);

    free(row);
}

void testApplyOccupationToRow(void)
{
    int l = 12;
    Occupation *row = (Occupation *)malloc(l * sizeof(Occupation));

    applyOccupationToRow(row, l, ICE);

    for (int i = 0; i < l; i++)
        TEST_ASSERT_EQUAL(ICE, row[i]);

    free(row);
}

void testApplyOccupationToSafeRowWithTrees(void)
{
    int l = 20;
    Occupation *row = (Occupation *)malloc(l * sizeof(Occupation));

    applyOccupationToRow(row, l, SAFE);

    int treeCount = 0;
    int safeCount = 0;

    for (int i = 0; i < l; i++)
        if (row[i] == TREE)
            treeCount++;
        else if (row[i] == SAFE)
            safeCount++;
        else
            TEST_FAIL_MESSAGE("Unexpected occupation type in safeRow : a safe row should have only trees and safezones"); // and a player but not here

    TEST_ASSERT_GREATER_THAN(0, treeCount);
    TEST_ASSERT_GREATER_THAN(0, safeCount);

    free(row);
}

void testCreateTrees(void)
{
    int l = 5;
    Occupation *row = (Occupation *)malloc(l * sizeof(Occupation));

    createTrees(row, l);

    int safeCount = 0;
    for (int i = 0; i < l; i++)
        if (row[i] == SAFE)
            safeCount++;

    TEST_ASSERT_LESS_OR_EQUAL(l, safeCount);

    free(row);
}

void testCreateGrid(void)
{
    int h = 10;
    int l = 20;
    int carMaxSize = 5; // Maybe use the global variable from headers ?

    Grid *grid = createGrid(h, l, carMaxSize);

    TEST_ASSERT_NOT_NULL(grid);
    TEST_ASSERT_NOT_NULL(grid->cases);
    TEST_ASSERT_NOT_NULL(grid->rowManagers);
    TEST_ASSERT_EQUAL(h, grid->height);
    TEST_ASSERT_EQUAL(l + 2 * carMaxSize, grid->length);

    for (int i = 0; i < h; i++)
    {
        TEST_ASSERT_NOT_NULL(grid->cases[i]);
        TEST_ASSERT_NOT_NULL(grid->rowManagers[i]);

        if (i == 0)
            TEST_ASSERT_EQUAL(SAFE, grid->rowManagers[i]->type);
        else
            TEST_ASSERT_EQUAL(ROAD, grid->rowManagers[i]->type); // The starting zone (till screen height) is only roads.

        TEST_ASSERT_TRUE(grid->rowManagers[i]->direction == 1 ||
                         grid->rowManagers[i]->direction == -1);

        TEST_ASSERT_GREATER_OR_EQUAL(25, grid->rowManagers[i]->speed);
        TEST_ASSERT_LESS_OR_EQUAL(60, grid->rowManagers[i]->speed);
    }

    destroyGrid(grid);
}

void testCreateGridWithNullValues(void) // We should never have this but we can test that it doesn't crash.
{
    Grid *grid = createGrid(0, 0, 0);

    TEST_ASSERT_NOT_NULL(grid);
    TEST_ASSERT_EQUAL(0, grid->height);
    TEST_ASSERT_EQUAL(0, grid->length);

    destroyGrid(grid);
}

void testCreateLargeGrid(void) // We could've made this for our SDL2 Game : a larger grid makes it easier to handle animation between coordinates.
{
    int h = 100;
    int l = 200;
    int carMaxSize = 10;

    Grid *grid = createGrid(h, l, carMaxSize);

    TEST_ASSERT_NOT_NULL(grid);
    TEST_ASSERT_EQUAL(h, grid->height);
    TEST_ASSERT_EQUAL(l + 2 * carMaxSize, grid->length);

    TEST_ASSERT_NOT_NULL(grid->cases[0]);
    TEST_ASSERT_NOT_NULL(grid->cases[h / 2]);
    TEST_ASSERT_NOT_NULL(grid->cases[h - 1]);

    TEST_ASSERT_NOT_NULL(grid->rowManagers[0]);
    TEST_ASSERT_NOT_NULL(grid->rowManagers[h / 2]);
    TEST_ASSERT_NOT_NULL(grid->rowManagers[h - 1]);

    destroyGrid(grid);
}
