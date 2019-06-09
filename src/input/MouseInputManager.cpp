#include "MouseInputManager"
#include "SceneSettings"
#include "GraphicsSettings"
#include "Options"
#include "Camera"
#include "ScreenResolution"

#include <glm/glm.hpp>

extern GLFWwindow* window;
extern Camera camera;
extern Camera shadowCamera;
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
  MouseInputManager& mouseInput = getInstance();

  if (options[OPT_SHOW_CURSOR])
    {
      mouseInput.lastX = x;
      mouseInput.lastY = y;
      glfwGetCursorPos(window, &cursorScreenX, &cursorScreenY);
      glm::vec3 viewDirection = camera.getDirection(); //normalized
      glm::vec3 viewHorizontal = camera.getRight(); //normalized
      glm::vec3 viewVertical = camera.getUp(); //normalized
      float fovRad = glm::radians(camera.getZoom());
      float verticalLength = std::tan(fovRad / 2) * NEAR_PLANE;
      float horizontalLength = verticalLength * screenResolution.getAspectRatio();
      viewHorizontal *= horizontalLength;
      viewVertical *= verticalLength;

      float halfScreenWidth = screenResolution.getWidth() / 2;
      float halfScreenHeight = screenResolution.getHeight() / 2;
      //map coordinates from [0 ; widthPixels or heightPixels] to the NDC space
      cursorScreenX -= halfScreenWidth;
      cursorScreenY -= halfScreenHeight;
      cursorScreenX /= halfScreenWidth;
      cursorScreenY /= halfScreenHeight;
      glm::vec3 newPosition = camera.getPosition() + viewDirection * NEAR_PLANE //get point on the near plane
                              + //horizontal pick shifting corresponds with the cursor horizontal direction
                              viewHorizontal * (float)cursorScreenX //shift picked cursor point horizontally
                              - //but vertical shifting is reflected (because window coordinate system Y axis is reflected)
                              viewVertical * (float)cursorScreenY;    //shift picked cursor point vertically
      mouseInput.cursorToNearPlaneWorldSpace = newPosition - camera.getPosition();

      return; //if the cursor is on the screen - bypass camera movement
    }

  if (firstMouseInput)
    {
      mouseInput.lastX = x;
      mouseInput.lastY = y;
      firstMouseInput = false;
    }

  //calculate velocities before update
  float dx = mouseInput.lastX - x;
  float dy = y - mouseInput.lastY; //must be reflected
  //then update
  mouseInput.lastX = x;
  mouseInput.lastY = y;
  camera.updateViewAcceleration(dx, dy);
  shadowCamera.updateViewAcceleration(dx, dy);
}

void MouseInputManager::scrollCallback(GLFWwindow *, double, double y)
{
  camera.processMouseScroll(y);
  shadowCamera.processMouseScroll(y);
}

void MouseInputManager::cursorClickCallback(GLFWwindow *window, int button, int action, int)
{
  MouseInputManager& mouseInput = getInstance();
  static bool mouseKeysPressed[GLFW_MOUSE_BUTTON_LAST];
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
      if (!mouseKeysPressed[GLFW_MOUSE_BUTTON_RIGHT])
        {
          options.toggle(OPT_SHOW_CURSOR);
          glfwSetInputMode(window, GLFW_CURSOR, options[OPT_SHOW_CURSOR] ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
          float halfScreenWidth = screenResolution.getWidth() / 2.0f;
          float halfScreenHeight = screenResolution.getHeight() / 2.0f;
          glfwSetCursorPos(window, halfScreenWidth, halfScreenHeight);
          mouseInput.lastX = halfScreenWidth;
          mouseInput.lastY = halfScreenHeight;
          mouseKeysPressed[GLFW_MOUSE_BUTTON_RIGHT] = true;
        }
    }
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    mouseKeysPressed[GLFW_MOUSE_BUTTON_RIGHT] = false;
}

void MouseInputManager::updateCursorMappingCoordinates(const Camera &camera, const map2D_f &landMap, const map2D_f& hillMap, const map2D_f& buildableMap)
{
  if (options[OPT_SHOW_CURSOR] && cursorToNearPlaneWorldSpace.y < 0.0f)
    {
      /* this variable is used to determine a cursor pick point on the world map where Y coordinate is 0.0
       * i.e. the higher camera is, the farther a tile on which a cursor is pointing
       * on the other side - the more "vertical" cursor world-space direction vector is, the nearer a tile
       */
      float cameraToCursorYposRatio = camera.getPosition().y / (-cursorToNearPlaneWorldSpace.y);

      bool cursorOutOfMap = false;
      cursorAbsX = glm::clamp((cursorToNearPlaneWorldSpace.x * cameraToCursorYposRatio) + camera.getPosition().x, -HALF_WORLD_WIDTH_F, HALF_WORLD_WIDTH_F);
      cursorAbsZ = glm::clamp((cursorToNearPlaneWorldSpace.z * cameraToCursorYposRatio) + camera.getPosition().z, -HALF_WORLD_HEIGHT_F, HALF_WORLD_HEIGHT_F);
      if (cursorAbsX == -HALF_WORLD_WIDTH || cursorAbsX == HALF_WORLD_WIDTH ||
          cursorAbsZ == -HALF_WORLD_HEIGHT || cursorAbsZ == HALF_WORLD_HEIGHT)
        cursorOutOfMap = true;
      if (cursorOutOfMap)
        {
          cursorTileName = "out of map";
          return;
        }
      cursorWorldX = (int)(WORLD_WIDTH + cursorAbsX) - HALF_WORLD_WIDTH;
      cursorWorldX = glm::clamp(cursorWorldX, 1, WORLD_WIDTH - 2);
      cursorWorldZ = (int)(WORLD_HEIGHT + cursorAbsZ) - HALF_WORLD_HEIGHT + 1;
      cursorWorldZ = glm::clamp(cursorWorldZ, 1, WORLD_HEIGHT - 1);
      if (buildableMap[cursorWorldZ][cursorWorldX] != 0)
        cursorTileName = "Land";
      else if (hillMap[cursorWorldZ][cursorWorldX] != 0 ||
               hillMap[cursorWorldZ-1][cursorWorldX] != 0 ||
               hillMap[cursorWorldZ-1][cursorWorldX+1] != 0 ||
               hillMap[cursorWorldZ][cursorWorldX+1] != 0)
        cursorTileName = "Hills";
      else
        {
          if (landMap[cursorWorldZ][cursorWorldX] == TILE_NO_RENDER_VALUE ||
              landMap[cursorWorldZ-1][cursorWorldX] == TILE_NO_RENDER_VALUE ||
              landMap[cursorWorldZ-1][cursorWorldX+1] == TILE_NO_RENDER_VALUE ||
              landMap[cursorWorldZ][cursorWorldX+1] == TILE_NO_RENDER_VALUE)
            cursorTileName = "Water";
          else
            cursorTileName = "Shore";
        }
    }
  else
    cursorTileName = "out of map";
}

int MouseInputManager::getCursorWorldX() const
{
  return cursorWorldX;
}

int MouseInputManager::getCursorWorldZ() const
{
  return cursorWorldZ;
}

const glm::vec3 &MouseInputManager::getCursorToNearPlaneWorldSpace() const
{
  return cursorToNearPlaneWorldSpace;
}

const std::string &MouseInputManager::getCursorTileName() const
{
  return cursorTileName;
}
