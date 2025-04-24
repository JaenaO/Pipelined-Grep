#ifndef MONITOR_H
#define MONITOR_H

#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

class boundedBuffer {
private:
  int CAPACITY;
  int* buffer;
  int tail;
  int head;
  int count;

  mutex pcLock;
  condition_variable notEmpty;
  condition_variable notFull;

public:
  boundedBuffer(int buffersize) {
    CAPACITY = buffersize;
    buffer = new int[CAPACITY];
    tail = 0;
    head = 0;
    count = 0;
  }
  void add(int number);
  int remove(void);
};

#endif // MONITOR_H