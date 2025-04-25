/* Note: This file is built on given starter code that implements a solution to the producer consumer problem using monitors*/

#include <iostream>
#include <thread>
#include <string>

#include <cmath>
#include <cstdlib>
#include <cassert>

#include <mutex>
#include <condition_variable>

#include "boundedBuffer.h"

using namespace std;
 
boundedBuffer::boundedBuffer(int buffersize){
   CAPACITY = buffersize;
   buffer = new string[CAPACITY];
   tail = 0;
   head = 0;
   count = 0;
}

/* Function used to add an item to the buffer */
void boundedBuffer::add( string str ) 
{
   /* Acquire the lock for critical section */
   unique_lock<mutex>  pcULock( pcLock );

   assert(count >= 0 && count <= CAPACITY);
   
   while (count >= CAPACITY ) {
      /* Wait until the buffer is not full */
      notFull.wait( pcULock );
   }

   /* insert the item at the tail end of the buffer */
   buffer[tail] = str;
   tail = (tail + 1) % CAPACITY;
   ++count;

   /* Wake up a consumer */
   /* Use notify_all( ) to wake up all consumers */
   notEmpty.notify_one( );
   
   /* Release the lock for critical section */
   pcULock.unlock( );

   return;
}

/* Function used to remove an item from the buffer */
string boundedBuffer::remove( void ) 
{
   /* Acquire the lock for critical section */
   unique_lock<mutex>  pcULock( pcLock );

   assert(count >= 0 && count <= CAPACITY);

   while (count <= 0 ) {
      /* Wait until the buffer is not full */
      notEmpty.wait( pcULock );
   }

   /* Delete an item at the head end of the buffer */
   string str = buffer[head];
   buffer[head] = "";
   head = (head + 1) % CAPACITY;
   --count;

   /* Wake up a producer */
   /* Use notify_all( ) to wake all producers */
   notFull.notify_one( );
   
   /* Release the lock for critical section */
   pcULock.unlock( );

   return str;
}