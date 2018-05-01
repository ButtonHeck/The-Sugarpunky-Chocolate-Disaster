#include "InputController.h"

extern GLFWwindow* window;
extern Camera camera;
extern glm::vec3 cursorToViewportDirection;
extern bool renderShadowOnTrees;
extern bool renderTreeModels;
extern bool animateWater;
extern bool renderDebugText;
extern bool recreateTerrain;
extern bool saveRequest;
extern bool loadRequest;
extern bool showBuildable;
extern bool showCursor;
extern int scr_width;
extern int scr_height;
extern float aspect_ratio;
bool keysPressed[GLFW_KEY_LAST];
bool mouseKeysPressed[GLFW_MOUSE_BUTTON_LAST];
bool firstMouseInput = true;
bool cursorActivatedButUncentered = true;
bool polygonLineMode = false;
float lastX, lastY;
double cursorScreenX = 0.0;
double cursorScreenY = 0.0;

void InputController::processKeyboard(float delta)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.processKeyboardInput(delta, FORWARD);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.processKeyboardInput(delta, BACKWARD);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.processKeyboardInput(delta, LEFT);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.processKeyboardInput(delta, RIGHT);
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    camera.processKeyboardInput(delta, DOWN);
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    camera.processKeyboardInput(delta, UP);

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
}

void InputController::cursorCallback(GLFWwindow *, double x, double y)
{
  if (showCursor)
    {
      if (cursorActivatedButUncentered)
        {
          glfwSetCursorPos(window, scr_width / 2.0f, scr_height / 2.0f);
          cursorActivatedButUncentered = false;
        }
      lastX = x;
      lastY = y;
      glfwGetCursorPos(window, &cursorScreenX, &cursorScreenY);
      glm::vec3 view = camera.getDirection();
      view = glm::normalize(view);
      glm::vec3 h = camera.getRight();
      h = glm::normalize(h);
      glm::vec3 v = camera.getUp();
      v = glm::normalize(v);
      float fovRad = glm::radians(camera.getZoom());
      float vLength = std::tan(fovRad / 2) * NEAR_PLANE;
      float hLength = vLength * aspect_ratio;
      h *= hLength;
      v *= vLength;
      cursorScreenX -= scr_width / 2;
      cursorScreenY -= scr_height / 2;
      cursorScreenX /= (scr_width / 2);
      cursorScreenY /= (scr_height / 2);
      glm::vec3 newPos = camera.getPosition() + view * NEAR_PLANE + h * (float)cursorScreenX - v * (float)cursorScreenY;
      glm::vec3 newDir = newPos - camera.getPosition();
      cursorToViewportDirection = newDir;
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
  camera.processMouseCursor(dx, dy);
}

void InputController::cursorClickCallback(GLFWwindow *window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
      if (!mouseKeysPressed[GLFW_MOUSE_BUTTON_RIGHT])
        {
          showCursor = !showCursor;
          glfwSetInputMode(window, GLFW_CURSOR, showCursor ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
          cursorActivatedButUncentered = showCursor;
          mouseKeysPressed[GLFW_MOUSE_BUTTON_RIGHT] = true;
        }
    }
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    mouseKeysPressed[GLFW_MOUSE_BUTTON_RIGHT] = false;
}

