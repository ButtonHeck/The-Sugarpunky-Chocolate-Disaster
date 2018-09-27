#ifndef MOUSEINPUTMANAGER_H
#define MOUSEINPUTMANAGER_H
#include "graphics/Camera.h"
#include <GLFW/glfw3.h>
#include "game/Options.h"
#include "game/ScreenResolution.h"
#include "generator/BaseMapGenerator.h"
#include "generator/HillsMapGenerator.h"
#include "generator/BuildableMapGenerator.h"

class MouseInputManager
{
public:
  static MouseInputManager& getInstance();
  static void cursorCallback(GLFWwindow*, double x, double y);
  static void cursorClickCallback(GLFWwindow*, int, int, int);
  void updateCursorMappingCoordinates(Camera& camera,
                         BaseMapGenerator* baseMapGenerator,
                         HillsMapGenerator* hillMapGenerator,
                         BuildableMapGenerator* buildableMapGenerator);
  int getCursorMapX() const;
  int getCursorMapZ() const;
  const glm::vec3& getCursorToViewportDirection() const;
  const std::string& getCursorTileName() const;
private:
  MouseInputManager() = default;
  glm::vec3 cursorToViewportDirection;
  float lastX, lastY;
  float cursorOnMapX = 0.0f;
  float cursorOnMapZ = 0.0f;
  int cursorOnMapCoordX = 0;
  int cursorOnMapCoordZ = 0;
  std::string cursorTileName = "Flat";
};

#endif // MOUSEINPUTMANAGER_H
