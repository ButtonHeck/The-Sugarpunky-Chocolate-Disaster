/*
 * Copyright 2019 Ilya Malgin
 * KeyboardManager.h
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
 * Purpose: contains declaration for KeyboardManager class
 * @version 0.1.0
 */

#pragma once

#include "Options"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>

class Camera;
class TheSunFacade;

/**
* @brief manager for all keyboard-driven events. Responsible for processing keyboard events, 
* binding different events and functions to dedicated keys
*/
class KeyboardManager
{
public:
  KeyboardManager(GLFWwindow* window, Camera& camera, Camera& shadowCamera, Options& options, TheSunFacade &sun) noexcept;
  void processInput();

private:
  void processKey(int keyCode, OPTION option);
  void processKey(int keyCode, std::function<void()> function);

  bool keysPressed[GLFW_KEY_LAST];
  GLFWwindow* window;
  Camera& camera;
  /** @todo remove from release version of the game */
  Camera& shadowCamera;
  Options& options;
  /** @todo remove from release version of the game */
  TheSunFacade& sun;
};
