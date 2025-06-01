#include "unity.h"
#include "player.h"
#include "gamestate.h"
#include <stdlib.h>

char moveKeys[] = {'z', 's', 'q', 'd'};
int xMoves[] = {0, 0, -1, 1};
int yMoves[] = {1, -1, 0, 0};
int lastMooves[] = {1, -1, 2, -2}; // Refactor in a 3-tuples array ?
int lenMovesArr = sizeof(moveKeys) / sizeof(char);

void testPlayerMoveKey(void)
{
    for (int i = 0; i < lenMovesArr; i++)
    {
        GameState *gs = initGameState(15, 20);
        gs->player->x = 10;
        gs->player->y = 10;
        gs->player->mouvementCooldown = 0;

        // We make a safezone around the player for him to be able to moove freely.
        for (int x = -1; x <= 1; x++)
        {
            for (int y = -1; y <= 1; y++)
            {
                int newX = gs->player->x + x;
                int newY = gs->player->y + y;
                if (newX >= 0 && newX < gs->grid->length && newY >= 0 && newY < gs->grid->height)
                    gs->grid->cases[newY][newX] = SAFE;
            }
        }

        bool hasMoved = false;
        int initialX = gs->player->x;
        int initialY = gs->player->y;

        playerMoveKey(moveKeys[i], gs, &hasMoved);

        TEST_ASSERT_TRUE(hasMoved);
        TEST_ASSERT_EQUAL(initialY + yMoves[i], gs->player->y);
        TEST_ASSERT_EQUAL(initialX + xMoves[i], gs->player->x);
        TEST_ASSERT_EQUAL(lastMooves[i], gs->player->lastMoove);

        destroyGameState(gs);
    }
} // Aim to check if the moveKey function works properly, we ignore the fact that the real game has Cars, etc.

int blockedX[] = {0, 0, 4, 26};
int blockedY[] = {19, 0, 0, 0};
bool expectGameOver[] = {false, true, false, false};

void testPlayerMoveKeyButWithBlockedCases(void)
{
    for (int i = 0; i < lenMovesArr; i++)
    {
        GameState *gs = initGameState(20, 20);
        bool hasMoved = false;

        gs->player->x = blockedX[i];
        gs->player->y = blockedY[i];
        gs->player->mouvementCooldown = 0;

        playerMoveKey(moveKeys[i], gs, &hasMoved);

        if (expectGameOver[i])
            TEST_ASSERT_TRUE(gs->gameOver);
        else
        {
            TEST_ASSERT_FALSE(gs->gameOver);
            TEST_ASSERT_FALSE(hasMoved);
            TEST_ASSERT_EQUAL(blockedX[i], gs->player->x);
            TEST_ASSERT_EQUAL(blockedY[i], gs->player->y);
        }

        destroyGameState(gs);
    }
}

void testPlayerMoveKeyShouldQuitWithF(void)
{
    GameState *gs = initGameState(10, 20);
    bool hasMoved = false;

    playerMoveKey('f', gs, &hasMoved);

    TEST_ASSERT_TRUE(gs->gameOver);

    destroyGameState(gs);
}

void testPlayerShouldNotMoveWithCooldown(void)
{
    GameState *gs = initGameState(10, 20);
    bool hasMoved = false;
    gs->player->mouvementCooldown = 5;
    int initialX = gs->player->x;
    int initialY = gs->player->y;

    playerMoveKey('z', gs, &hasMoved);

    TEST_ASSERT_FALSE(hasMoved);
    TEST_ASSERT_EQUAL(initialX, gs->player->x);
    TEST_ASSERT_EQUAL(initialY, gs->player->y);

    destroyGameState(gs);
}

void testPlayerMoveKeyShouldAlwaysAllowQuitting(void)
{
    GameState *gs = initGameState(10, 20);
    bool hasMoved = false;
    gs->player->mouvementCooldown = 5;

    playerMoveKey('f', gs, &hasMoved);

    TEST_ASSERT_TRUE(gs->gameOver);

    destroyGameState(gs);
}

void testPlayerMoveKeyShouldIgnoreInvalidKeys(void)
{
    GameState *gs = initGameState(10, 20);
    bool hasMoved = false;
    int initialX = gs->player->x;
    int initialY = gs->player->y;

    playerMoveKey('x', gs, &hasMoved);

    TEST_ASSERT_FALSE(hasMoved);
    TEST_ASSERT_EQUAL(initialX, gs->player->x);
    TEST_ASSERT_EQUAL(initialY, gs->player->y);
    TEST_ASSERT_FALSE(gs->gameOver);

    destroyGameState(gs);
}

void testUpdateIceShouldNotSlideWithCooldown(void)
{
    GameState *gs = initGameState(10, 20);

    gs->grid->rowManagers[1]->type = ICE;
    gs->player->y = 1;
    gs->player->lastMoove = 1;
    gs->player->slidingCooldown = 10;

    int initialY = gs->player->y;

    updateIce(gs);

    TEST_ASSERT_EQUAL(initialY, gs->player->y);
    TEST_ASSERT_EQUAL(9, gs->player->slidingCooldown);

    destroyGameState(gs);
}

void testUpdateIceShouldNotAffectPlayerIfHeIsNotOnIce(void)
{
    GameState *gs = initGameState(10, 20);

    gs->player->slidingCooldown = 0;
    int initialY = gs->player->y;
    int initialCooldown = gs->player->slidingCooldown;

    updateIce(gs);

    TEST_ASSERT_EQUAL(initialY, gs->player->y);
    TEST_ASSERT_EQUAL(initialCooldown, gs->player->slidingCooldown);

    destroyGameState(gs);
}

void testUpdateIceShouldHandleNegativeY(void) // It shouldn't crash even if we shouldn't have this in game.
{
    GameState *gs = initGameState(10, 20);

    gs->player->y = -1;
    updateIce(gs);

    TEST_ASSERT_EQUAL(-1, gs->player->y);

    destroyGameState(gs);
}

int slideArr[] = {-1, 2, -2, 1};

void testUpdateIceShouldSlide(void)
{
    for (int i = 0; i < 3; i++)
    {
        GameState *gs = initGameState(10, 20);

        gs->grid->rowManagers[2]->type = ICE;
        gs->grid->rowManagers[1]->type = ICE;
        gs->grid->rowManagers[0]->type = ICE;
        gs->player->y = 1;
        gs->player->lastMoove = slideArr[i];
        gs->player->slidingCooldown = 0;

        int initialY = gs->player->y;
        int initialX = gs->player->x;

        updateIce(gs);

        if (slideArr[i] == -1)
            TEST_ASSERT_EQUAL(initialY - 1, gs->player->y);
        else if (slideArr[i] == 2)
            TEST_ASSERT_EQUAL(initialX - 1, gs->player->x);
        else if (slideArr[i] == -2)
            TEST_ASSERT_EQUAL(initialX + 1, gs->player->x);
        else
            TEST_ASSERT_EQUAL(initialY + 1, gs->player->y);

        TEST_ASSERT_EQUAL(36, gs->player->slidingCooldown);

        destroyGameState(gs);
    }
}