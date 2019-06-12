#pragma once

#include "BenchmarkTimer"

#include <GLFW/glfw3.h>
#include <chrono>

class Timer
{
public:
  Timer() noexcept
    :
      frameTime(chronoClock::now()),
      currentTime(frameTime)
  {}

  float tick()
  {
    nowTime = glfwGetTime();
    delta = nowTime - lastTime;
    lastTime = nowTime;
    ++frames;
    currentTime = chronoClock::now();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - frameTime).count() > 1000)
      {
        frameTime = currentTime;
        fps = frames;
        if (updateCount > 1)
          {
            BenchmarkTimer::printFrameBenchmarks(updateCount, fps);
            BenchmarkTimer::resetFrameBenchmarks();
          }
        frames = 0;
        ++updateCount;
      }
    return delta;
  }
  unsigned int getFPS() noexcept
  {
    return fps;
  }

private:
  using chronoClock = std::chrono::high_resolution_clock;

  float lastTime, nowTime, delta;
  decltype(chronoClock::now()) frameTime, currentTime;
  unsigned int frames = 0, fps = 0, updateCount = 0;
};
