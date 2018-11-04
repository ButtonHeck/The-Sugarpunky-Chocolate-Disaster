#ifndef KEYBOARDMANAGER_H
#define KEYBOARDMANAGER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>
#include "game/Camera.h"
#include "util/Settings.h"
#include "game/Options.h"
#include "util/BenchmarkTimer.h"

class KeyboardManager
{
public:
  KeyboardManager(GLFWwindow* window, Camera& camera, Options& options);
  void processInput();
private:
  bool keysPressed[GLFW_KEY_LAST];
  void processKey(int keyCode, OPTION option);
  void processKey(int keyCode, std::function<void()> function);
  GLFWwindow* window;
  Camera& camera;
  Options& options;
};

#endif // KEYBOARDMANAGER_H
