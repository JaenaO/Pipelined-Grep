/* Note: This file is built on given starter code that implements a solution to the producer consumer problem using monitors*/

#include <iostream>
#include <thread>
#include <string>

#include <cmath>
#include <cstdlib>
#include <cassert>

#include <mutex>
#include <condition_variable>

#include "monitor.h"

using namespace std;

class Monitor {
private:
   
   int CAPACITY; /* Size of the buffer */

   int* buffer;

   /* The two endpoints of the queue */
   int tail;
   int head;

   /* Number of items currently in the buffer */
   int count;

   /* A lock to provide mutual exclusion */
   mutex pcLock;

   /* To condition variables for producer(s) and concumer(s) to synchronize with each other */
   condition_variable notEmpty;
   condition_variable notFull;

public:
   Monitor(int buffersize){
      CAPACITY = buffersize;
      buffer = new int[CAPACITY];
      tail = 0;
      head = 0;
      count = 0;

   }
   /* Function used to add an item to the buffer */
   void add( int number ) 
   {
      /* Acquire the lock for critical section */
      unique_lock<mutex>  pcULock( pcLock );

      assert(count >= 0 && count <= CAPACITY);
      
      while (count >= CAPACITY ) {
         /* Wait until the buffer is not full */
         notFull.wait( pcULock );
      }

      /* insert the item at the tail end of the buffer */
      buffer[tail] = number;
      tail = (tail + 1) % CAPACITY;
      ++count;

      cout << "Number added is " << number << endl;

      /* Wake up a consumer */
      /* Use notify_all( ) to wake up all consumers */
      notEmpty.notify_one( );
      
      /* Release the lock for critical section */
      pcULock.unlock( );

      return;
   }

   /* Function used to remove an item from the buffer */
   int remove( void ) 
   {
      /* Acquire the lock for critical section */
      unique_lock<mutex>  pcULock( pcLock );

      assert(count >= 0 && count <= CAPACITY);
   
      while (count <= 0 ) {
         /* Wait until the buffer is not full */
         notEmpty.wait( pcULock );
      }
   
      /* Delete an item at the head end of the buffer */
      int number = buffer[head];
      buffer[head] = 0;
      head = (head + 1) % CAPACITY;
      --count;

      assert( number > 0 );
      
      cout << "Number removed is " << number << endl;

      /* Wake up a producer */
      /* Use notify_all( ) to wake all producers */
      notFull.notify_one( );
      
      /* Release the lock for critical section */
      pcULock.unlock( );

      return number;
   }
};