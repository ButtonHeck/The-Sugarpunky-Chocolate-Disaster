#ifndef THREAD_H
#define THREAD_H
#include <thread>
#include <pthread.h>
#include <iostream>
#include <cstring>

class Thread
{
public:
  Thread() = default;
  virtual ~Thread();
protected:
  std::thread* thread;
};

#endif // THREAD_H
