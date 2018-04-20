#include "InputController.h"

extern GLFWwindow* window;
extern Camera cam;
extern bool renderShadowOnTrees;
extern bool renderTreeModels;
extern bool animateWater;
extern bool renderDebugText;
extern bool recreateTerrain;
extern bool saveRequest;
extern bool loadRequest;
extern bool showBuildable;
extern bool showMouse;
bool keysPressed[GLFW_KEY_LAST];
bool firstMouseInput = true;
bool polygonLineMode = false;
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
  //polygon mode
  if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
    {
      if (!keysPressed[GLFW_KEY_F1])
        {
          polygonLineMode = !polygonLineMode;
          glPolygonMode(GL_FRONT_AND_BACK, polygonLineMode ? GL_LINE : GL_FILL);
          keysPressed[GLFW_KEY_F1] = true;
        }
    }
  if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_RELEASE)
    keysPressed[GLFW_KEY_F1] = false;

  //render shadow on tree leaves
  if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
    {
      if (!keysPressed[GLFW_KEY_F2])
        {
          renderShadowOnTrees = !renderShadowOnTrees;
          keysPressed[GLFW_KEY_F2] = true;
        }
    }
  if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_RELEASE)
    keysPressed[GLFW_KEY_F2] = false;

  //animate water
  if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
    {
      if (!keysPressed[GLFW_KEY_F3])
        {
          animateWater = !animateWater;
          keysPressed[GLFW_KEY_F3] = true;
        }
    }
  if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_RELEASE)
    keysPressed[GLFW_KEY_F3] = false;

  //render trees
  if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS)
    {
      if (!keysPressed[GLFW_KEY_F4])
        {
          renderTreeModels = !renderTreeModels;
          keysPressed[GLFW_KEY_F4] = true;
        }
    }
  if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_RELEASE)
    keysPressed[GLFW_KEY_F4] = false;

  //render debug text
  if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS)
    {
      if (!keysPressed[GLFW_KEY_F5])
        {
          renderDebugText = !renderDebugText;
          keysPressed[GLFW_KEY_F5] = true;
        }
    }
  if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_RELEASE)
    keysPressed[GLFW_KEY_F5] = false;

  //show buildable
  if (glfwGetKey(window, GLFW_KEY_F6) == GLFW_PRESS)
    {
      if (!keysPressed[GLFW_KEY_F6])
        {
          showBuildable = !showBuildable;
          keysPressed[GLFW_KEY_F6] = true;
        }
    }
  if (glfwGetKey(window, GLFW_KEY_F6) == GLFW_RELEASE)
    keysPressed[GLFW_KEY_F6] = false;

  //recreate terrain
  if (glfwGetKey(window, GLFW_KEY_F9) == GLFW_PRESS)
    {
      if (!keysPressed[GLFW_KEY_F9])
        {
          recreateTerrain = !recreateTerrain;
          keysPressed[GLFW_KEY_F9] = true;
        }
    }
  if (glfwGetKey(window, GLFW_KEY_F9) == GLFW_RELEASE)
    keysPressed[GLFW_KEY_F9] = false;

  //save request
  if (glfwGetKey(window, GLFW_KEY_F10) == GLFW_PRESS)
    {
      if (!keysPressed[GLFW_KEY_F10])
        {
          saveRequest = !saveRequest;
          keysPressed[GLFW_KEY_F10] = true;
        }
    }
  if (glfwGetKey(window, GLFW_KEY_F10) == GLFW_RELEASE)
    keysPressed[GLFW_KEY_F10] = false;

  //load request
  if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS)
    {
      if (!keysPressed[GLFW_KEY_F11])
        {
          loadRequest = !loadRequest;
          keysPressed[GLFW_KEY_F11] = true;
        }
    }
  if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_RELEASE)
    keysPressed[GLFW_KEY_F11] = false;

  //show mouse
  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
      if (!keysPressed[GLFW_KEY_LEFT_CONTROL])
        {
          showMouse = !showMouse;
          glfwSetInputMode(window, GLFW_CURSOR, showMouse ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
          keysPressed[GLFW_KEY_LEFT_CONTROL] = true;
        }
    }
  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
    keysPressed[GLFW_KEY_LEFT_CONTROL] = false;
}

void InputController::cursorCallback(GLFWwindow *, double x, double y)
{
  if (showMouse)
    {
      lastX = x;
      lastY = y;
      return;
    }
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

