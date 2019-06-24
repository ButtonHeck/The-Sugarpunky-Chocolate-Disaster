/*
 * Copyright 2019 Ilya Malgin
 * main.cpp
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
 * Purpose: contains application entry point code and main() function
 * @version 0.1.0
 */

#include "Game"
#include "ScreenResolution"
#include "Logger"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/**
* @brief temporary global variable for debugging purposes
* @todo remove this from release version of the game and put it in the Sun submodule instead
*/
float debug_sunSpeed = 2.0f;

int main()
{
  //initialize GLFW stuff
  glfwSetErrorCallback([](int, const char* msg)
  {
      Logger::log("Error with GLFW library: %\n", msg);
    });
  if (!glfwInit())
    throw std::runtime_error("Error while loading GLFW\n");
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef _DEBUG
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
  ScreenResolution screenResolution(vidmode->width, vidmode->height);
  GLFWwindow* window = glfwCreateWindow(screenResolution.getWidth(), screenResolution.getHeight(), "The Sugarpunky Chocolate Disaster", monitor, 0);
  glfwMakeContextCurrent(window);

  //initialize GLEW
  glewExperimental = GL_TRUE;
  glewInit();

#ifdef _DEBUG
  Logger::log("%\n", glfwGetVersionString());
  GLint flags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
  {
	  glEnable(GL_DEBUG_OUTPUT);
	  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	  glDebugMessageCallback(Logger::glDebugCallback, nullptr);
	  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
  }
#endif

  //explicitly make non-current from this thread, as the context will mainly be used in the game child thread
  glfwMakeContextCurrent(NULL);

  std::thread gameThread([&]()
  {
	  glfwMakeContextCurrent(window);
	  /*
	   * compiler gives warning if I try to allocate Game object in stack memory,
	   * on the other hand, using smart pointer for this only because of warning is overkill,
	   * so, plain old new/delete is ok here
	   */
	  Game* game = new Game(window, screenResolution);
	  game->setup();
	  while (!glfwWindowShouldClose(window))
	  {
		  game->loop();
	  }
	  delete game;
  });

  //don't know why pollEvents function is working as we nullified current context for this thread, but it works.
  while (!glfwWindowShouldClose(window))
  {
	  glfwPollEvents();
	  //~0.5ms delay of mouse/key input is applicable
	  std::this_thread::sleep_for(std::chrono::microseconds(500));
  }
  gameThread.join();

  //cleanup
  glfwDestroyWindow(window);
  glfwTerminate();
}
