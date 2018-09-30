#include "WaterAnimationUpdater.h"

WaterAnimationUpdater::WaterAnimationUpdater(GLFWwindow *window, Options &options, WaterMapGenerator *generator)
  :
    window(window),
    options(options),
    waterMapGenerator(generator)
{
  thread = new std::thread(&WaterAnimationUpdater::updateWaterAnimationFrame, this);
}

bool WaterAnimationUpdater::isWorking() const
{
  return working;
}

bool WaterAnimationUpdater::isReady() const
{
  return ready;
}

void WaterAnimationUpdater::setNewFrameNeed(bool need)
{
  newFrameNeed = need;
}

void WaterAnimationUpdater::updateWaterAnimationFrame()
{
  while(!glfwWindowShouldClose(window))
    {
      if (newFrameNeed &&
          options.get(OPT_ANIMATE_WATER) &&
          options.get(OPT_DRAW_WATER))
        {
          ready = false;
          waterMapGenerator->updateAnimationFrame(options);
          ready = true;
          working = true;
        }
      else
        {
          working = false;
          std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
      std::this_thread::yield();
    }
}
