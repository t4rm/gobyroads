#include "unity.h"
#include "gamestate.h"
#include <stdlib.h>

void testInitGameState(void)
{
    GameState *gs = initGameState(10, 20);

    TEST_ASSERT_NOT_NULL(gs);
    TEST_ASSERT_NOT_NULL(gs->cars);
    TEST_ASSERT_NOT_NULL(gs->grid);
    TEST_ASSERT_NOT_NULL(gs->player);
    TEST_ASSERT_EQUAL(6, gs->carMaxSize);
    TEST_ASSERT_EQUAL((20 + 2 * 6 + 1) / 2, gs->player->x); // Middle of the viewable screen
    TEST_ASSERT_EQUAL(0, gs->player->y);
    TEST_ASSERT_EQUAL(0, gs->player->mouvementCooldown);
    TEST_ASSERT_EQUAL(0, gs->player->afk);
    TEST_ASSERT_EQUAL(0, gs->score);
    TEST_ASSERT_EQUAL(0, gs->backwardMovements);
    TEST_ASSERT_EQUAL(0, gs->highestLineReached);
    TEST_ASSERT_FALSE(gs->gameOver);

    destroyGameState(gs);
}

void testInitGameStateDifferentDimensions(void) // It should work for any dimensions.
{
    GameState *gs = initGameState(5, 15);

    TEST_ASSERT_NOT_NULL(gs);
    TEST_ASSERT_EQUAL(5, gs->grid->height);
    TEST_ASSERT_EQUAL(15 + 2 * 6, gs->grid->length);
    TEST_ASSERT_EQUAL((15 + 2 * 6 + 1) / 2, gs->player->x);

    destroyGameState(gs);
}

void testInitialSafeZones(void)
{
    GameState *gs = initGameState(10, 20);

    TEST_ASSERT_EQUAL(SAFE, gs->grid->cases[gs->player->y][gs->player->x]);
    TEST_ASSERT_EQUAL(SAFE, gs->grid->rowManagers[3]->type);

    destroyGameState(gs);
}

void testUpdateTrainShouldHandleRailToWarning(void)
{
    GameState *gs = initGameState(5, 10);

    gs->grid->rowManagers[1]->type = RAIL;
    gs->grid->rowManagers[1]->speed = 50;
    gs->grid->rowManagers[1]->cooldown = 50;

    TrainState result = updateTrain(gs->grid);

    TEST_ASSERT_EQUAL(HORN, result);
    TEST_ASSERT_EQUAL(WARNING, gs->grid->rowManagers[1]->type);
    TEST_ASSERT_EQUAL(0, gs->grid->rowManagers[1]->cooldown);
    TEST_ASSERT_EQUAL(2 * 60, gs->grid->rowManagers[1]->speed);

    destroyGameState(gs);
}

void testUpdateTrainShouldHandleWarningToTrain(void)
{
    GameState *gs = initGameState(5, 10);

    gs->grid->rowManagers[1]->type = WARNING;
    gs->grid->rowManagers[1]->speed = 120;
    gs->grid->rowManagers[1]->cooldown = 120;

    TrainState result = updateTrain(gs->grid);

    TEST_ASSERT_EQUAL(PASSING, result);
    TEST_ASSERT_EQUAL(TRAIN, gs->grid->rowManagers[1]->type);
    TEST_ASSERT_EQUAL(0, gs->grid->rowManagers[1]->cooldown);
    TEST_ASSERT_EQUAL(8 * 60, gs->grid->rowManagers[1]->speed);

    destroyGameState(gs);
}

void testUpdateTrainShouldHandleTrainToRail(void)
{
    GameState *gs = initGameState(5, 10);

    gs->grid->rowManagers[1]->type = TRAIN;
    gs->grid->rowManagers[1]->speed = 480;
    gs->grid->rowManagers[1]->cooldown = 480;

    TrainState result = updateTrain(gs->grid);

    TEST_ASSERT_EQUAL(AWAY, result);
    TEST_ASSERT_EQUAL(RAIL, gs->grid->rowManagers[1]->type);
    TEST_ASSERT_EQUAL(0, gs->grid->rowManagers[1]->cooldown);
    TEST_ASSERT_GREATER_OR_EQUAL(16 * 60, gs->grid->rowManagers[1]->speed);

    destroyGameState(gs);
}

void testUpdateTrainShouldHandleAwayState(void)
{
    GameState *gs = initGameState(5, 10);

    TrainState result = updateTrain(gs->grid);

    TEST_ASSERT_EQUAL(AWAY, result);

    destroyGameState(gs);
}

void testHandleCollisionShouldDetectOutOfMapFromTheLeft(void)
{
    GameState *gs = initGameState(5, 10);

    gs->player->x = gs->carMaxSize - 1;

    CollisionState result = handleCollision(gs);

    TEST_ASSERT_EQUAL(OUT_OF_MAP, result);
    TEST_ASSERT_TRUE(gs->gameOver);

    destroyGameState(gs);
}

void testHandleCollisionShouldDetectOutOfMapFromTheRight(void)
{
    GameState *gs = initGameState(5, 10);

    gs->player->x = gs->grid->length - gs->carMaxSize + 2;

    CollisionState result = handleCollision(gs);

    TEST_ASSERT_EQUAL(OUT_OF_MAP, result);
    TEST_ASSERT_TRUE(gs->gameOver);

    destroyGameState(gs);
}

void testHandleCollisionShouldDetectOutOfMapFromTheBottom(void)
{
    GameState *gs = initGameState(5, 10);

    gs->player->y = -1;
    CollisionState result = handleCollision(gs);

    TEST_ASSERT_EQUAL(OUT_OF_MAP, result);
    TEST_ASSERT_TRUE(gs->gameOver);

    destroyGameState(gs);
}

Occupation collisionArray[] = {CAR_LEFT, CAR_RIGHT, WATER, TRAIN};
int lenColArray = sizeof(collisionArray) / sizeof(Occupation); // Or hardcoded 4.

void testHandleCollisionDetection(void)
{
    for (int i = 0; i < lenColArray; i++)
    {
        GameState *gs = initGameState(5, 10);

        gs->grid->cases[gs->player->y][gs->player->x] = collisionArray[i];

        CollisionState result = handleCollision(gs);

        if (collisionArray[i] == WATER)
            TEST_ASSERT_EQUAL(SPLASHED, result);
        else
            TEST_ASSERT_EQUAL(CRASHED, result);
        TEST_ASSERT_TRUE(gs->gameOver);

        destroyGameState(gs);
    }
}

void testHandleScoreIncrementation(void)
{
    GameState *gs = initGameState(5, 10);

    int initialScore = gs->score;
    bool hasMoved = false;
    playerMoveKey('z', gs, &hasMoved);
    handleScore(gs);

    TEST_ASSERT_TRUE(hasMoved);
    TEST_ASSERT_EQUAL(initialScore + 1, gs->score);
    TEST_ASSERT_EQUAL(1, gs->highestLineReached);

    destroyGameState(gs);
}

void testHandleScoreNoMovement(void)
{
    GameState *gs = initGameState(5, 10);

    int initialScore = gs->score;

    handleScore(gs);

    TEST_ASSERT_EQUAL(initialScore, gs->score);

    destroyGameState(gs);
}

void testHandleScoreBackwardNotDecrementing(void)
{
    GameState *gs = initGameState(5, 10);

    gs->player->y = 3;
    gs->highestLineReached = 3;
    handleScore(gs);
    int scoreAfterAdvance = gs->score;

    gs->player->y = 1;
    handleScore(gs);

    TEST_ASSERT_EQUAL(scoreAfterAdvance, gs->score);
    destroyGameState(gs);
}

void testScrollingShouldTrigger(void)
{
    GameState *gs = initGameState(10, 20);

    gs->player->y = 3;

    scrolling(gs, false);

    TEST_ASSERT_EQUAL(2, gs->player->y);
    TEST_ASSERT_EQUAL(2, gs->highestLineReached);

    destroyGameState(gs);
}

void testScrollingShouldNotTrigger(void)
{
    GameState *gs = initGameState(10, 20);

    gs->player->y = 2;
    int initialY = gs->player->y;

    scrolling(gs, false);

    TEST_ASSERT_EQUAL(initialY, gs->player->y);

    destroyGameState(gs);
}