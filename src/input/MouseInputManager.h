#pragma once

#include "TypeAliases"

#include <GLFW/glfw3.h>

class Camera;

class MouseInputManager
{
public:
  static MouseInputManager& getInstance();
  static void setCallbacks(GLFWwindow* window) noexcept;
  void updateCursorMappingCoordinates(const Camera& camera, const map2D_f &landMap, const map2D_f &hillMap, const map2D_f &buildableMap);
  int getCursorWorldX() const noexcept;
  int getCursorWorldZ() const noexcept;
  const glm::vec3& getCursorToNearPlaneWorldSpace() const noexcept;
  const std::string& getCursorTileName() const noexcept;

private:
  MouseInputManager() = default;
  static void cursorMoveCallback(GLFWwindow*, double x, double y);
  static void scrollCallback(GLFWwindow*, double, double y);
  static void cursorClickCallback(GLFWwindow*, int, int, int);

  static GLFWwindow* window;
  glm::vec3 cursorToNearPlaneWorldSpace;
  float lastX, lastY;
  float cursorAbsX = 0.0f;
  float cursorAbsZ = 0.0f;
  int cursorWorldX = 0;
  int cursorWorldZ = 0;
  std::string cursorTileName = "Land";
};
