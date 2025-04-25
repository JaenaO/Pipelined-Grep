#ifndef BOUNDED_BUFFER_H
#define BOUNDED_BUFFER_H

#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

class boundedBuffer {
private:
  int CAPACITY;
  string* buffer;
  int tail;
  int head;
  int count;

  mutex pcLock;
  condition_variable notEmpty;
  condition_variable notFull;

public:
  boundedBuffer(int buffersize);
  void add(string str);
  string remove(void);
};

#endif // BOUNDED_BUFFER_H