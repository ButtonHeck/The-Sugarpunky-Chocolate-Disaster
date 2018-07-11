#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include "graphics/Camera.h"
#include "game/Settings.h"
#include "game/Options.h"
#include "generator/BaseMapGenerator.h"
#include "generator/HillsMapGenerator.h"
#include "generator/BuildableMapGenerator.h"

class InputController
{
public:
  InputController() = default;
  void processKeyboard();
  void processKeyboardCamera(float delta, std::vector<std::vector<float>>& hillsMap);
  static void cursorCallback(GLFWwindow*, double x, double y);
  static void cursorClickCallback(GLFWwindow*, int, int, int);
  void updateCursorMappingCoordinates(Camera& camera,
                         BaseMapGenerator* baseMapGenerator,
                         HillsMapGenerator* hillMapGenerator,
                         BuildableMapGenerator* buildableMapGenerator);
  int getCursorMapX() const;
  int getCursorMapZ() const;
  const std::string& getCursorTileName() const;
private:
  float cursorOnMapX = 0.0f;
  float cursorOnMapZ = 0.0f;
  int cursorOnMapCoordX = 0;
  int cursorOnMapCoordZ = 0;
  std::string cursorTileName = "Flat";
};

#endif // INPUTCONTROLLER_H
