#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Camera.h"
#include "Settings.h"
#include <glm/glm.hpp>
#include "BaseMapGenerator.h"
#include "HillsMapGenerator.h"
#include "BuildableMapGenerator.h"

class InputController
{
public:
  InputController() = default;
  void processKeyboard(float delta);
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
