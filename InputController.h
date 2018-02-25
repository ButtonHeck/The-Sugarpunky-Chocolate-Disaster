#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Camera.h"

class InputController
{
public:
  InputController() = default;
  void processKeyboard(float delta);
  static void cursorCallback(GLFWwindow*, double x, double y);
};

#endif // INPUTCONTROLLER_H
