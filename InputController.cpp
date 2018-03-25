#include "InputController.h"

extern GLFWwindow* window;
extern Camera cam;
extern bool shadow;
bool firstMouseInput = true;
float lastX, lastY;

void InputController::processKeyboard(float delta)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    cam.processKeyboardInput(delta, FORWARD);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    cam.processKeyboardInput(delta, BACKWARD);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cam.processKeyboardInput(delta, LEFT);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cam.processKeyboardInput(delta, RIGHT);
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    cam.processKeyboardInput(delta, DOWN);
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    cam.processKeyboardInput(delta, UP);
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
    shadow = true;
  if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
    shadow = false;
}

void InputController::cursorCallback(GLFWwindow *, double x, double y)
{
  if (firstMouseInput)
    {
      lastX = x;
      lastY = y;
      firstMouseInput = false;
    }
  float dx = lastX - x;
  float dy = y - lastY;
  lastX = x;
  lastY = y;
  cam.processMouseCursor(dx, dy);
}

