#ifndef KEYBOARDMANAGER_H
#define KEYBOARDMANAGER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>
#include "graphics/Camera.h"
#include "game/Options.h"
#include "util/BenchmarkTimer.h"
#include "game/world/ambience/theSun/TheSunFacade.h"

class KeyboardManager
{
public:
  KeyboardManager(GLFWwindow* window, Camera& camera, Camera& shadowCamera, Options& options, TheSunFacade &sun);
  void processInput();
private:
  bool keysPressed[GLFW_KEY_LAST];
  void processKey(int keyCode, OPTION option);
  void processKey(int keyCode, std::function<void()> function);
  GLFWwindow* window;
  Camera& camera;
  Camera& shadowCamera;
  Options& options;
  TheSunFacade& sun;
};

#endif // KEYBOARDMANAGER_H
