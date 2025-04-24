/* This program implements a solution to the producer consumer problem using monitors */

#include <iostream>
#include <thread>
#include <string>

#include <cmath>
#include <cstdlib>
#include <cassert>

#include <mutex>
#include <condition_variable>

using namespace std;

/* Data structure for storing the items produced until consumed */

/* Capacity of the bounded buffer */
const int CAPACITY = 25;

/* Array to store items - used to implement a FIFO queue using circular buffer */
int buffer[CAPACITY];

/* The two endpoints of the queue */
int tail;
int head;

/* Number of items currently in the buffer */
int count;

/* Number of producers and consumers in the system */
const int PRODUCERS = 2;
const int CONSUMERS = 2;

/* Arrays to store producer and consumer threads */
thread producers[PRODUCERS];
thread consumers[CONSUMERS];

/* Variables for process synchronization */

/* A lock to provide mutual exclusion */
mutex pcLock;

/* To condition variables for producer(s) and concumer(s) to synchronize with each other */
condition_variable notEmpty;
condition_variable notFull;

/* Function used to add an item to the buffer */
void add(int number)
{
    /* Acquire the lock for critical section */
    unique_lock<mutex> pcULock(pcLock);

    assert(count >= 0 && count <= CAPACITY);

    while (count >= CAPACITY)
    {
        /* Wait until the buffer is not full */
        notFull.wait(pcULock);
    }

    /* insert the item at the tail end of the buffer */
    buffer[tail] = number;
    tail = (tail + 1) % CAPACITY;
    ++count;

    cout << "Number added is " << number << endl;

    /* Wake up a consumer */
    /* Use notify_all( ) to wake up all consumers */
    notEmpty.notify_one();

    /* Release the lock for critical section */
    pcULock.unlock();

    return;
}

/* Function used to remove an item from the buffer */
int remove(void)
{
    /* Acquire the lock for critical section */
    unique_lock<mutex> pcULock(pcLock);

    assert(count >= 0 && count <= CAPACITY);

    while (count <= 0)
    {
        /* Wait until the buffer is not full */
        notEmpty.wait(pcULock);
    }

    /* Delete an item at the head end of the buffer */
    int number = buffer[head];
    buffer[head] = 0;
    head = (head + 1) % CAPACITY;
    --count;

    assert(number > 0);

    cout << "Number removed is " << number << endl;

    /* Wake up a producer */
    /* Use notify_all( ) to wake all producers */
    notFull.notify_one();

    /* Release the lock for critical section */
    pcULock.unlock();

    return number;
}

/* Driver code for a producer */
void producer(void)
{
    while (1)
    {
        add(rand() % 100 + 1);
    }
}

/* Driver code for a consumer */
void consumer(void)
{
    while (1)
    {
        int number = remove();
    }
}

/* The main function */
int main(void)
{

    /* Initialize the buffer */
    for (int i = 0; i < CAPACITY; ++i)
    {
        buffer[i] = 0;
    }

    /* Initialize the variables associated with the buffer */
    head = 0;
    tail = 0;
    count = 0;

    /* Create producer and consumer threads */
    for (int i = 0; i < PRODUCERS; ++i)
    {
        producers[i] = thread(producer);
    }
    for (int i = 0; i < CONSUMERS; ++i)
    {
        consumers[i] = thread(consumer);
    }

    /* Wait for all threads to terminate */
    for (int i = 0; i < PRODUCERS; ++i)
    {
        producers[i].join();
    }
    for (int i = 0; i < CONSUMERS; ++i)
    {
        consumers[i].join();
    }

    return 0;
}
