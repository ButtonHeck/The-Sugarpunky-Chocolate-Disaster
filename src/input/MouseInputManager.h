/*
 * Copyright 2019 Ilya Malgin
 * MouseInputManager.h
 * This file is part of The Sugarpunky Chocolate Disaster project
 *
 * The Sugarpunky Chocolate Disaster project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The Sugarpunky Chocolate Disaster project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * See <http://www.gnu.org/licenses/>
 *
 * Purpose: contains declaration for MouseInputManager class
 * @version 0.1.0
 */

#pragma once

#include "TypeAliases"

class Camera;
class Options;
class ScreenResolution;
class GLFWwindow;

/**
* @brief manager for mouse related events. Responsible for handling mouse callbacks and cursor picking.
* Implements singleton pattern
*/
class MouseInputManager
{
public:
  static MouseInputManager& getInstance();
  static void initialize(GLFWwindow* window, 
						 Options& options, 
						 const ScreenResolution& screenResolution,
						 Camera& camera,
						 Camera& shadowCamera) noexcept;
  static void setCallbacks() noexcept;
  void updateCursorMappingCoordinates(const map2D_f &landMap, const map2D_f &hillMap, const map2D_f &buildableMap);
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
  static Options* options;
  static const ScreenResolution* screenResolution;
  static Camera* camera;
  /** @todo remove this in release version of the game */
  static Camera* shadowCamera;

  glm::vec3 cursorToNearPlaneWorldSpace;
  float lastX, lastY;
  float cursorAbsX = 0.0f;
  float cursorAbsZ = 0.0f;
  int cursorWorldX = 0;
  int cursorWorldZ = 0;
  std::string cursorTileName = "Land";
};
