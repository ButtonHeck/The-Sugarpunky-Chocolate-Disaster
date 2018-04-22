#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Camera.h"

class InputController
{
public:
  InputController() = default;
  void processKeyboard(float delta);
  static void cursorCallback(GLFWwindow*, double x, double y);
  static void cursorClickCallback(GLFWwindow*, int, int, int);
};

#endif // INPUTCONTROLLER_H
