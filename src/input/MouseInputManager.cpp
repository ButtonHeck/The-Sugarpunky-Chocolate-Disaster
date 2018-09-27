#include "MouseInputManager.h"

extern GLFWwindow* window;
extern Camera camera;
extern Options options;
extern ScreenResolution screenResolution;

/*
Singleton for mouse input, NOT THREAD SAFE!
*/
MouseInputManager &MouseInputManager::getInstance()
{
  static MouseInputManager instance;
  return instance;
}

void MouseInputManager::cursorCallback(GLFWwindow *, double x, double y)
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
  camera.processMouseCursor(dx, dy);
}

void MouseInputManager::cursorClickCallback(GLFWwindow *window, int button, int action, int)
{
  MouseInputManager& mouseInput = MouseInputManager::getInstance();
  static bool mouseKeysPressed[GLFW_MOUSE_BUTTON_LAST];
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
      if (!mouseKeysPressed[GLFW_MOUSE_BUTTON_RIGHT])
        {
          options.switchOpt(OPT_SHOW_CURSOR);
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

void MouseInputManager::updateCursorMappingCoordinates(Camera &camera, BaseMapGenerator *baseMapGenerator, HillsMapGenerator *hillMapGenerator, BuildableMapGenerator *buildableMapGenerator)
{
  if (options.get(OPT_SHOW_CURSOR) && cursorToViewportDirection.y < 0.0f)
    {
      float ratio = camera.getPosition().y / (-cursorToViewportDirection.y);
      bool cursorOutOfMap = false;
      cursorOnMapX = glm::clamp((cursorToViewportDirection.x * ratio) + camera.getPosition().x, -(float)HALF_WORLD_WIDTH, (float)HALF_WORLD_WIDTH);
      cursorOnMapZ = glm::clamp((cursorToViewportDirection.z * ratio) + camera.getPosition().z, -(float)HALF_WORLD_HEIGHT, (float)HALF_WORLD_HEIGHT);
      if (cursorOnMapX == -HALF_WORLD_WIDTH || cursorOnMapX == HALF_WORLD_WIDTH ||
          cursorOnMapZ == -HALF_WORLD_HEIGHT || cursorOnMapZ == HALF_WORLD_HEIGHT)
        cursorOutOfMap = true;
      if (cursorOutOfMap)
        {
          cursorTileName = "out of map";
          return;
        }
      cursorOnMapCoordX = (int)(WORLD_WIDTH + cursorOnMapX) - HALF_WORLD_WIDTH;
      cursorOnMapCoordX = glm::clamp(cursorOnMapCoordX, 1, WORLD_WIDTH - 2);
      cursorOnMapCoordZ = (int)(WORLD_HEIGHT + cursorOnMapZ) - HALF_WORLD_HEIGHT + 1;
      cursorOnMapCoordZ = glm::clamp(cursorOnMapCoordZ, 1, WORLD_HEIGHT - 1);
      if (buildableMapGenerator->getMap()[cursorOnMapCoordZ][cursorOnMapCoordX] != 0)
        cursorTileName = "Flat";
      else if (hillMapGenerator->getMap()[cursorOnMapCoordZ][cursorOnMapCoordX] != 0 ||
               hillMapGenerator->getMap()[cursorOnMapCoordZ-1][cursorOnMapCoordX] != 0 ||
               hillMapGenerator->getMap()[cursorOnMapCoordZ-1][cursorOnMapCoordX+1] != 0 ||
               hillMapGenerator->getMap()[cursorOnMapCoordZ][cursorOnMapCoordX+1] != 0)
        cursorTileName = "Hills";
      else
        {
          if (baseMapGenerator->getMap()[cursorOnMapCoordZ][cursorOnMapCoordX] == TILE_NO_RENDER_VALUE ||
              baseMapGenerator->getMap()[cursorOnMapCoordZ-1][cursorOnMapCoordX] == TILE_NO_RENDER_VALUE ||
              baseMapGenerator->getMap()[cursorOnMapCoordZ-1][cursorOnMapCoordX+1] == TILE_NO_RENDER_VALUE ||
              baseMapGenerator->getMap()[cursorOnMapCoordZ][cursorOnMapCoordX+1] == TILE_NO_RENDER_VALUE)
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
  return cursorOnMapCoordX;
}

int MouseInputManager::getCursorMapZ() const
{
  return cursorOnMapCoordZ;
}

const glm::vec3 &MouseInputManager::getCursorToViewportDirection() const
{
  return cursorToViewportDirection;
}

const std::string &MouseInputManager::getCursorTileName() const
{
  return cursorTileName;
}
