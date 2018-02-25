#ifndef TIMER_H
#define TIMER_H
#include <GLFW/glfw3.h>

class Timer
{
public:
  Timer() = default;
  float tick()
  {
    nowTime = glfwGetTime();
    delta = nowTime - lastTime;
    lastTime = nowTime;
    return delta;
  }
private:
  float lastTime, nowTime, delta;
};

#endif // TIMER_H
