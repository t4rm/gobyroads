#include "unity.h"
#include "car_queue.h"
#include <stdlib.h>

// simple car with default color and no variant.
Car *newCar(int x, int y, int sz)
{
    Car *c = malloc(sizeof(Car));
    c->x = x;
    c->y = y;
    c->size = sz;
    c->direction = 1;
    c->color = 0;
    c->variant = '\0';
    return c;
}

void testCreateEmptyQueue()
{
    CarQueue *carQueue = createCarQueue();
    TEST_ASSERT_NOT_NULL(carQueue);
    TEST_ASSERT_EQUAL(0, carQueue->size);
    TEST_ASSERT_NULL(carQueue->head);
    TEST_ASSERT_NULL(carQueue->tail);
    destroyCarQueue(carQueue);
}

void testAddSingleCar()
{
    CarQueue *carQueue = createCarQueue();
    Car *car = newCar(5, 2, 3);

    addLastCar(carQueue, car);

    TEST_ASSERT_EQUAL(1, carQueue->size);
    TEST_ASSERT_NOT_NULL(carQueue->head);
    TEST_ASSERT_NOT_NULL(carQueue->tail);

    // equal but with pointers, check content
    TEST_ASSERT_EQUAL_PTR(car, carQueue->head->car);

    destroyCarQueue(carQueue);
}

void testAddMultipleCars()
{
    CarQueue *queue = createCarQueue();

    Car *c1 = newCar(1, 1, 2);
    Car *c2 = newCar(2, 2, 3);
    Car *c3 = newCar(3, 3, 4);

    addLastCar(queue, c1);
    addLastCar(queue, c2);
    addLastCar(queue, c3);

    // basic assertions
    TEST_ASSERT_EQUAL(3, queue->size);
    TEST_ASSERT_EQUAL_PTR(c1, queue->head->car);
    TEST_ASSERT_EQUAL_PTR(c3, queue->tail->car);

    destroyCarQueue(queue);
}

void testRemoveFirstOfTwo()
{
    CarQueue *queue = createCarQueue();
    Car *a = newCar(10, 5, 2);
    Car *b = newCar(11, 6, 3);

    addLastCar(queue, a);
    addLastCar(queue, b);

    removeFirstCar(queue);

    TEST_ASSERT_EQUAL(1, queue->size);
    TEST_ASSERT_EQUAL_PTR(b, queue->head->car);

    destroyCarQueue(queue);
}

void testRemoveFirstEmpty()
{
    CarQueue *queue = createCarQueue();
    removeFirstCar(queue);
    TEST_ASSERT_EQUAL(0, queue->size);
    destroyCarQueue(queue);
}

void testRemoveRow1()
{
    CarQueue *carQueue = createCarQueue();
    addLastCar(carQueue, newCar(1, 1, 2));
    addLastCar(carQueue, newCar(2, 2, 2));
    addLastCar(carQueue, newCar(3, 1, 2));
    addLastCar(carQueue, newCar(4, 3, 2));

    removeRowCar(carQueue, 1); // remove all y = 1

    TEST_ASSERT_EQUAL(2, carQueue->size);
    CarElement *e = carQueue->head;
    TEST_ASSERT_EQUAL(2, e->car->y);
    e = e->next;
    TEST_ASSERT_EQUAL(3, e->car->y);

    destroyCarQueue(carQueue);
}

void testRemoveMissingRow()
{
    CarQueue *queue = createCarQueue();
    Car *car = newCar(5, 1, 2);
    addLastCar(queue, car);

    removeRowCar(queue, 999); // nothing to remove

    TEST_ASSERT_EQUAL(1, queue->size);
    destroyCarQueue(queue);
}

void testRemoveEmptyQueueRow()
{
    CarQueue *carQueue = createCarQueue();
    removeRowCar(carQueue, 1);
    TEST_ASSERT_EQUAL(0, carQueue->size);
    destroyCarQueue(carQueue);
}

void testRemoveSingle()
{
    CarQueue *carQueue = createCarQueue();
    Car *c = newCar(1, 1, 2);
    addLastCar(carQueue, c);
    removeFirstCar(carQueue);

    TEST_ASSERT_NULL(carQueue->head);
    TEST_ASSERT_NULL(carQueue->tail);
    TEST_ASSERT_EQUAL(0, carQueue->size);

    destroyCarQueue(carQueue);
}

// edge case
void testRemoveMiddle()
{
    CarQueue *carQueue = createCarQueue();
    Car *c1 = newCar(1, 1, 1);
    Car *c2 = newCar(2, 2, 1);
    Car *c3 = newCar(3, 3, 1);

    addLastCar(carQueue, c1);
    addLastCar(carQueue, c2);
    addLastCar(carQueue, c3);

    removeRowCar(carQueue, 2); // remove c2

    TEST_ASSERT_EQUAL(2, carQueue->size);
    TEST_ASSERT_EQUAL_PTR(carQueue->head->car, c1);
    TEST_ASSERT_EQUAL_PTR(carQueue->tail->car, c3);
    TEST_ASSERT_EQUAL_PTR(carQueue->head->next, carQueue->tail);
    TEST_ASSERT_EQUAL_PTR(carQueue->tail->previous, carQueue->head);

    destroyCarQueue(carQueue);
}
