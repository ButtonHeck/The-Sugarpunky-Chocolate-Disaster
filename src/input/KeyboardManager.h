#ifndef KEYBOARDMANAGER_H
#define KEYBOARDMANAGER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>
#include "game/Camera.h"
#include "util/Settings.h"
#include "game/Options.h"

class KeyboardManager
{
public:
  KeyboardManager(GLFWwindow* window, Camera& camera, Options& options);
  void processInput(float delta, std::vector<std::vector<float> > &hillsMap);
private:
  bool keysPressed[GLFW_KEY_LAST];
  void processKey(int keyCode, OPTION option);
  void processKey(int keyCode, std::function<void()> function);
  GLFWwindow* window;
  Camera& camera;
  Options& options;
};

#endif // KEYBOARDMANAGER_H
