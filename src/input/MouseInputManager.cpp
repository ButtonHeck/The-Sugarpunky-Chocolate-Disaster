#include "input/MouseInputManager.h"

extern GLFWwindow* window;
extern Camera camera;
extern Options options;
extern ScreenResolution screenResolution;

/*
Singleton for mouse input, C++11 guarantees thread-safe instantiation
*/
MouseInputManager &MouseInputManager::getInstance()
{
  static MouseInputManager instance;
  return instance;
}

void MouseInputManager::setCallbacks(GLFWwindow *window)
{
  glfwSetCursorPosCallback(window, cursorMoveCallback);
  glfwSetMouseButtonCallback(window, cursorClickCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetScrollCallback(window, scrollCallback);
}

void MouseInputManager::cursorMoveCallback(GLFWwindow *, double x, double y)
{
  static bool firstMouseInput = true;
  static double cursorScreenX = 0.0;
  static double cursorScreenY = 0.0;
  MouseInputManager& mouseInput = MouseInputManager::getInstance();
  if (options.get(OPT_SHOW_CURSOR))
    {
      mouseInput.lastX = x;
      mouseInput.lastY = y;
      glfwGetCursorPos(window, &cursorScreenX, &cursorScreenY);
      glm::vec3 view = camera.getDirection();
      view = glm::normalize(view);
      glm::vec3 h = camera.getRight();
      h = glm::normalize(h);
      glm::vec3 v = camera.getUp();
      v = glm::normalize(v);
      float fovRad = glm::radians(camera.getZoom());
      float vLength = std::tan(fovRad / 2) * NEAR_PLANE;
      float hLength = vLength * screenResolution.getAspectRatio();
      h *= hLength;
      v *= vLength;
      cursorScreenX -= screenResolution.getWidth() / 2;
      cursorScreenY -= screenResolution.getHeight() / 2;
      cursorScreenX /= (screenResolution.getWidth() / 2);
      cursorScreenY /= (screenResolution.getHeight() / 2);
      glm::vec3 newPos = camera.getPosition() + view * NEAR_PLANE + h * (float)cursorScreenX - v * (float)cursorScreenY;
      glm::vec3 newDir = newPos - camera.getPosition();
      mouseInput.cursorToViewportDirection = newDir;
      return;
    }
  if (firstMouseInput)
    {
      mouseInput.lastX = x;
      mouseInput.lastY = y;
      firstMouseInput = false;
    }
  float dx = mouseInput.lastX - x;
  float dy = y - mouseInput.lastY;
  mouseInput.lastX = x;
  mouseInput.lastY = y;
  camera.updateAcceleration(dx, dy);
}

void MouseInputManager::scrollCallback(GLFWwindow *, double, double y)
{
  camera.processMouseScroll(y);
}

void MouseInputManager::cursorClickCallback(GLFWwindow *window, int button, int action, int)
{
  MouseInputManager& mouseInput = MouseInputManager::getInstance();
  static bool mouseKeysPressed[GLFW_MOUSE_BUTTON_LAST];
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
      if (!mouseKeysPressed[GLFW_MOUSE_BUTTON_RIGHT])
        {
          options.toggle(OPT_SHOW_CURSOR);
          glfwSetInputMode(window, GLFW_CURSOR, options.get(OPT_SHOW_CURSOR) ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
          glfwSetCursorPos(window, screenResolution.getWidth() / 2.0f, screenResolution.getHeight() / 2.0f);
          mouseInput.lastX = screenResolution.getWidth() / 2.0f;
          mouseInput.lastY = screenResolution.getHeight() / 2.0f;
          mouseKeysPressed[GLFW_MOUSE_BUTTON_RIGHT] = true;
        }
    }
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    mouseKeysPressed[GLFW_MOUSE_BUTTON_RIGHT] = false;
}

void MouseInputManager::updateCursorMappingCoordinates(Camera &camera,
                                                       const map2D_f &baseMap,
                                                       const map2D_f& hillMap,
                                                       const map2D_f& buildableMap)
{
  BENCHMARK("MouseInputManager: update cursor", true);
  if (options.get(OPT_SHOW_CURSOR) && cursorToViewportDirection.y < 0.0f)
    {
      float ratio = camera.getPosition().y / (-cursorToViewportDirection.y);
      bool cursorOutOfMap = false;
      cursorAbsX = glm::clamp((cursorToViewportDirection.x * ratio) + camera.getPosition().x, -HALF_WORLD_WIDTH_F, HALF_WORLD_WIDTH_F);
      cursorAbsZ = glm::clamp((cursorToViewportDirection.z * ratio) + camera.getPosition().z, -HALF_WORLD_HEIGHT_F, HALF_WORLD_HEIGHT_F);
      if (cursorAbsX == -HALF_WORLD_WIDTH || cursorAbsX == HALF_WORLD_WIDTH ||
          cursorAbsZ == -HALF_WORLD_HEIGHT || cursorAbsZ == HALF_WORLD_HEIGHT)
        cursorOutOfMap = true;
      if (cursorOutOfMap)
        {
          cursorTileName = "out of map";
          return;
        }
      cursorMapX = (int)(WORLD_WIDTH + cursorAbsX) - HALF_WORLD_WIDTH;
      cursorMapX = glm::clamp(cursorMapX, 1, WORLD_WIDTH - 2);
      cursorMapZ = (int)(WORLD_HEIGHT + cursorAbsZ) - HALF_WORLD_HEIGHT + 1;
      cursorMapZ = glm::clamp(cursorMapZ, 1, WORLD_HEIGHT - 1);
      if (buildableMap[cursorMapZ][cursorMapX] != 0)
        cursorTileName = "Flat";
      else if (hillMap[cursorMapZ][cursorMapX] != 0 ||
               hillMap[cursorMapZ-1][cursorMapX] != 0 ||
               hillMap[cursorMapZ-1][cursorMapX+1] != 0 ||
               hillMap[cursorMapZ][cursorMapX+1] != 0)
        cursorTileName = "Hills";
      else
        {
          if (baseMap[cursorMapZ][cursorMapX] == TILE_NO_RENDER_VALUE ||
              baseMap[cursorMapZ-1][cursorMapX] == TILE_NO_RENDER_VALUE ||
              baseMap[cursorMapZ-1][cursorMapX+1] == TILE_NO_RENDER_VALUE ||
              baseMap[cursorMapZ][cursorMapX+1] == TILE_NO_RENDER_VALUE)
            cursorTileName = "Water";
          else
            cursorTileName = "Shore";
        }
    }
  else
    cursorTileName = "out of map";
}

int MouseInputManager::getCursorMapX() const
{
  return cursorMapX;
}

int MouseInputManager::getCursorMapZ() const
{
  return cursorMapZ;
}

const glm::vec3 &MouseInputManager::getCursorToViewportDirection() const
{
  return cursorToViewportDirection;
}

const std::string &MouseInputManager::getCursorTileName() const
{
  return cursorTileName;
}
