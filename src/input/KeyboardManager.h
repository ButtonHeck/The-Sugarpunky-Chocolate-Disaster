#pragma once

#include "Camera"
#include "Options"
#include "TheSunFacade"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>

class KeyboardManager
{
public:
  KeyboardManager(GLFWwindow* window, Camera& camera, Camera& shadowCamera, Options& options, TheSunFacade &sun);
  void processInput();

private:
  void processKey(int keyCode, OPTION option);
  void processKey(int keyCode, std::function<void()> function);

  bool keysPressed[GLFW_KEY_LAST];
  GLFWwindow* window;
  Camera& camera;
  Camera& shadowCamera;
  Options& options;
  TheSunFacade& sun;
};
