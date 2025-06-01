#include "unity.h"
#include "car.h"
#include "gamestate.h"
#include <stdlib.h>

Occupation arrayCarTest[] = {ROAD, WATER, SAFE};
int lenArrCarTest = sizeof(arrayCarTest) / sizeof(Occupation); // Or hardcoded 3.

void testAddCar(void)
{
    for (int i = 0; i < lenArrCarTest; i++)
    {
        GameState *gs = initGameState(10, 20);
        int initialCarCount = gs->cars->size;

        gs->grid->rowManagers[1]->type = arrayCarTest[i];

        addCar(gs, 1, gs->grid->rowManagers[1], gs->grid->length);

        if (arrayCarTest[i] == SAFE)
            TEST_ASSERT_EQUAL(initialCarCount, gs->cars->size);
        else
            TEST_ASSERT_GREATER_THAN(initialCarCount, gs->cars->size);

        destroyGameState(gs);
    }
}

void testAddCarNegativeY(void)
{
    GameState *gs = initGameState(10, 20);
    int initialCarCount = gs->cars->size;

    addCar(gs, -1, gs->grid->rowManagers[0], gs->grid->length);

    TEST_ASSERT_EQUAL(initialCarCount, gs->cars->size);

    destroyGameState(gs);
}

void testAddCarTooHighY(void)
{
    GameState *gs = initGameState(10, 20);
    int initialCarCount = gs->cars->size;

    addCar(gs, gs->grid->height, gs->grid->rowManagers[0], gs->grid->length);

    TEST_ASSERT_EQUAL(initialCarCount, gs->cars->size);

    destroyGameState(gs);
}

void testCarPropertyAfterAddCar(void)
{
    GameState *gs = initGameState(10, 20);

    gs->grid->rowManagers[1]->type = ROAD;
    gs->grid->rowManagers[1]->direction = 1;

    addCar(gs, 1, gs->grid->rowManagers[1], gs->grid->length);

    TEST_ASSERT_GREATER_THAN(0, gs->cars->size);

    Car *firstCar = gs->cars->head->car;
    TEST_ASSERT_EQUAL(1, firstCar->y);
    TEST_ASSERT_GREATER_OR_EQUAL(1, firstCar->size);
    TEST_ASSERT_LESS_THAN(COLOR_COUNT, firstCar->color);

    destroyGameState(gs);
}

Car *createTestCar(int direction)
{
    Car *testCar = (Car *)malloc(sizeof(Car));
    testCar->x = 10;
    testCar->y = 1;
    testCar->size = 2;
    testCar->direction = direction;
    testCar->color = 0;
    testCar->variant = '\0';
    return testCar;
}

void testUpdateCarsWhenCooldownReached(void)
{
    GameState *gs = initGameState(10, 20);
    Car *testCar = createTestCar(1);
    addLastCar(gs->cars, testCar);

    gs->grid->rowManagers[1]->type = ROAD;
    gs->grid->rowManagers[1]->cooldown = gs->grid->rowManagers[1]->speed;

    int initialX = testCar->x;

    updateCars(gs);

    TEST_ASSERT_EQUAL(initialX + 1, testCar->x);

    destroyGameState(gs);
}

void testUpdateCarsWhenCooldownNotReached(void)
{
    GameState *gs = initGameState(10, 20);

    Car *testCar = createTestCar(1);
    addLastCar(gs->cars, testCar);

    gs->grid->rowManagers[1]->type = ROAD;
    gs->grid->rowManagers[1]->cooldown = 1;
    int initialX = testCar->x;

    updateCars(gs);

    TEST_ASSERT_EQUAL(initialX, testCar->x);
    TEST_ASSERT_EQUAL(2, gs->grid->rowManagers[1]->cooldown);

    destroyGameState(gs);
}

void testUpdateCarsShouldMoveInNegativeDirection(void)
{
    GameState *gs = initGameState(10, 20);

    Car *testCar = createTestCar(-1);

    addLastCar(gs->cars, testCar);

    gs->grid->rowManagers[1]->type = ROAD;
    gs->grid->rowManagers[1]->direction = -1;
    gs->grid->rowManagers[1]->cooldown = gs->grid->rowManagers[1]->speed;

    int initialX = testCar->x;

    updateCars(gs);

    TEST_ASSERT_EQUAL(initialX - 1, testCar->x);

    destroyGameState(gs);
}

void testUpdateCarsShouldUpdateGrid(void)
{
    GameState *gs = initGameState(10, 20);

    Car *testCar = createTestCar(1);

    addLastCar(gs->cars, testCar);

    gs->grid->rowManagers[1]->type = ROAD;

    updateCars(gs);

    bool carFound = false;
    for (int x = testCar->x; x < testCar->x + testCar->size; x++)
        if (gs->grid->cases[testCar->y][x] == CAR_LEFT || gs->grid->cases[testCar->y][x] == CAR_RIGHT)
            carFound = true;

    TEST_ASSERT_TRUE(carFound);

    destroyGameState(gs);
}

void testUpdateCarsWithMultipleCars(void)
{
    GameState *gs = initGameState(10, 20);

    Car *car1 = (Car *)malloc(sizeof(Car));
    car1->x = 5;
    car1->y = 1;
    car1->size = 2;
    car1->direction = 1;
    car1->color = 0;
    car1->variant = '\0';

    Car *car2 = (Car *)malloc(sizeof(Car));
    car2->x = 15;
    car2->y = 1;
    car2->size = 3;
    car2->direction = 1;
    car2->color = 1;
    car2->variant = '\0'; // Maybe refactorise this in util function

    addLastCar(gs->cars, car1);
    addLastCar(gs->cars, car2);

    gs->grid->rowManagers[1]->type = ROAD;
    gs->grid->rowManagers[1]->cooldown = gs->grid->rowManagers[1]->speed;

    int initialX1 = car1->x;
    int initialX2 = car2->x;

    updateCars(gs);

    TEST_ASSERT_EQUAL(initialX1 + 1, car1->x);
    TEST_ASSERT_EQUAL(initialX2 + 1, car2->x);

    destroyGameState(gs);
}