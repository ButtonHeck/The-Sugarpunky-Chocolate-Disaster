#ifndef WATERANIMATIONUPDATER_H
#define WATERANIMATIONUPDATER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include "thread/Thread.h"
#include "game/Options.h"
#include "generator/WaterMapGenerator.h"

class WaterAnimationUpdater : public Thread
{
public:
  WaterAnimationUpdater(GLFWwindow* window, Options& options, WaterMapGenerator* generator);
  ~WaterAnimationUpdater() = default;
  bool isWorking() const;
  bool isReady() const;
  void setNewFrameNeed(bool need);
private:
  void updateWaterAnimationFrame();
  GLFWwindow* window;
  Options& options;
  WaterMapGenerator* waterMapGenerator;
  volatile bool ready = false, newFrameNeed = true;
  bool working = true;
};

#endif // WATERANIMATIONUPDATER_H
