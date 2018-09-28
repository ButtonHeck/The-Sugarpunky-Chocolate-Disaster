#include "game/Thread.h"

Thread::~Thread()
{
  thread->join();
  delete thread;
}
