#ifndef TIMER_H
#define TIMER_H
#include <GLFW/glfw3.h>
#include <chrono>
#include "timer/BenchmarkTimer.h"
#include "generator/WaterMapGenerator.h"

class Timer
{
public:
  Timer()
    :
      frameTime(std::chrono::high_resolution_clock::now()),
      currentTime(frameTime)
  {}

  float tick()
  {
    nowTime = glfwGetTime();
    delta = nowTime - lastTime;
    lastTime = nowTime;
    ++frames;
    currentTime = std::chrono::high_resolution_clock::now();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - frameTime).count() > 1000)
      {
        frameTime = currentTime;
        fps = frames;
        if (updateCount > 1)
          {
            BenchmarkTimer::printBenchmarksPerFrame(updateCount, fps);
            BenchmarkTimer::clearBenchmarksPerFrameValues();
          }
        frames = 0;
        ++updateCount;
      }
    return delta;
  }
  unsigned int getFPS()
  {
    return fps;
  }
private:
  float lastTime, nowTime, delta;
  decltype(std::chrono::high_resolution_clock::now()) frameTime, currentTime;
  unsigned int frames = 0, fps = 0, updateCount = 0;
};

#endif // TIMER_H
