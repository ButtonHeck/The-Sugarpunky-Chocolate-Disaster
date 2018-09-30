#include "thread/Thread.h"

Thread::~Thread()
{
  thread->join();
  delete thread;
}
