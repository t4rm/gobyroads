#include "unity.h"

// car_queue.c
void testCreateEmptyQueue(void);
void testAddSingleCar(void);
void testAddMultipleCars(void);
void testRemoveFirstOfTwo(void);
void testRemoveFirstEmpty(void);
void testRemoveRow1(void);
void testRemoveMissingRow(void);
void testRemoveEmptyQueueRow(void);
void testRemoveSingle(void);
void testRemoveMiddle(void);

// map.c
void testCreateRowManager(void);
void testCreateRow(void);
void testCreateSafeRowWithTrees(void);
void testApplyOccupationToRow(void);
void testApplyOccupationToSafeRowWithTrees(void);
void testCreateTrees(void);
void testCreateGrid(void);
void testCreateGridWithNullValues(void);
void testCreateLargeGrid(void);

// car.c
void testAddCar(void);
void testAddCarNegativeY(void);
void testAddCarTooHighY(void);
void testCarPropertyAfterAddCar(void);
void testUpdateCarsWhenCooldownReached(void);
void testUpdateCarsWhenCooldownNotReached(void);
void testUpdateCarsShouldMoveInNegativeDirection(void);
void testUpdateCarsShouldUpdateGrid(void);
void testUpdateCarsWithMultipleCars(void);

// gamestate.c
void testInitGameState(void);
void testInitGameStateDifferentDimensions(void);
void testInitialSafeZones(void);
void testUpdateTrainShouldHandleRailToWarning(void);
void testUpdateTrainShouldHandleWarningToTrain(void);
void testUpdateTrainShouldHandleTrainToRail(void);
void testUpdateTrainShouldHandleAwayState(void);
void testHandleCollisionShouldDetectOutOfMapFromTheLeft(void);
void testHandleCollisionShouldDetectOutOfMapFromTheRight(void);
void testHandleCollisionShouldDetectOutOfMapFromTheBottom(void);
void testHandleCollisionDetection(void);
void testHandleScoreIncrementation(void);
void testHandleScoreNoMovement(void);
void testHandleScoreBackwardNotDecrementing(void);
void testScrollingShouldTrigger(void);
void testScrollingShouldNotTrigger(void);

// player.c
void testPlayerMoveKey(void);
void testPlayerMoveKeyShouldQuitWithF(void);
void testPlayerMoveKeyButWithBlockedCases(void);
void testPlayerShouldNotMoveWithCooldown(void);
void testPlayerMoveKeyShouldAlwaysAllowQuitting(void);
void testPlayerMoveKeyShouldIgnoreInvalidKeys(void);
void testUpdateIceShouldSlide(void);
void testUpdateIceShouldNotSlideWithCooldown(void);
void testUpdateIceShouldNotAffectPlayerIfHeIsNotOnIce(void);
void testUpdateIceShouldHandleNegativeY(void);

void setUp(void) {}
void tearDown(void) {}

int main(void)
{
    UNITY_BEGIN();

    // car_queue
    RUN_TEST(testCreateEmptyQueue);
    RUN_TEST(testAddSingleCar);
    RUN_TEST(testAddMultipleCars);
    RUN_TEST(testRemoveFirstOfTwo);
    RUN_TEST(testRemoveFirstEmpty);
    RUN_TEST(testRemoveRow1);
    RUN_TEST(testRemoveMissingRow);
    RUN_TEST(testRemoveEmptyQueueRow);
    RUN_TEST(testRemoveSingle);
    RUN_TEST(testRemoveMiddle);

    // map
    RUN_TEST(testCreateRowManager);
    RUN_TEST(testCreateRow);
    RUN_TEST(testCreateSafeRowWithTrees);
    RUN_TEST(testApplyOccupationToRow);
    RUN_TEST(testApplyOccupationToSafeRowWithTrees);
    RUN_TEST(testCreateTrees);
    RUN_TEST(testCreateGrid);
    RUN_TEST(testCreateGridWithNullValues);
    RUN_TEST(testCreateLargeGrid);

    // car
    RUN_TEST(testAddCar);
    RUN_TEST(testAddCarNegativeY);
    RUN_TEST(testAddCarTooHighY);
    RUN_TEST(testCarPropertyAfterAddCar);
    RUN_TEST(testUpdateCarsWhenCooldownReached);
    RUN_TEST(testUpdateCarsWhenCooldownNotReached);
    RUN_TEST(testUpdateCarsShouldMoveInNegativeDirection);
    RUN_TEST(testUpdateCarsShouldUpdateGrid);

    // gamestate
    RUN_TEST(testInitGameState);
    RUN_TEST(testInitGameStateDifferentDimensions);
    RUN_TEST(testInitialSafeZones);
    RUN_TEST(testUpdateTrainShouldHandleRailToWarning);
    RUN_TEST(testUpdateTrainShouldHandleWarningToTrain);
    RUN_TEST(testUpdateTrainShouldHandleTrainToRail);
    RUN_TEST(testUpdateTrainShouldHandleAwayState);
    RUN_TEST(testHandleCollisionShouldDetectOutOfMapFromTheLeft);
    RUN_TEST(testHandleCollisionShouldDetectOutOfMapFromTheRight);
    RUN_TEST(testHandleCollisionShouldDetectOutOfMapFromTheBottom);
    RUN_TEST(testHandleCollisionDetection);
    RUN_TEST(testHandleScoreIncrementation);
    RUN_TEST(testHandleScoreNoMovement);
    RUN_TEST(testHandleScoreBackwardNotDecrementing);
    RUN_TEST(testScrollingShouldTrigger);
    RUN_TEST(testScrollingShouldNotTrigger);

    // player
    RUN_TEST(testPlayerMoveKey);
    RUN_TEST(testPlayerMoveKeyShouldQuitWithF);
    RUN_TEST(testPlayerMoveKeyButWithBlockedCases);
    RUN_TEST(testPlayerMoveKeyShouldQuitWithF);
    RUN_TEST(testPlayerShouldNotMoveWithCooldown);
    RUN_TEST(testPlayerMoveKeyShouldAlwaysAllowQuitting);
    RUN_TEST(testPlayerMoveKeyShouldIgnoreInvalidKeys);
    RUN_TEST(testUpdateIceShouldSlide);
    RUN_TEST(testUpdateIceShouldNotSlideWithCooldown);
    RUN_TEST(testUpdateIceShouldNotAffectPlayerIfHeIsNotOnIce);
    RUN_TEST(testUpdateIceShouldHandleNegativeY);

    return UNITY_END();
}