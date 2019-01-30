#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "game/Game.h"
#include "game/Options.h"
#include "graphics/ScreenResolution.h"
#include "util/Logger.h"
#include "graphics/Camera.h"

GLFWwindow* window;
ScreenResolution screenResolution;
Camera camera(glm::vec3(0.0f, 12.0f, 0.0f));
Camera shadowCamera(glm::vec3(0.0f, 12.0f, 0.0f));
Options options;
Game* game;

float debug_sunSpeed = 2.0f;

int main()
{
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
  screenResolution.updateResolution(vidmode->width, vidmode->height);
  window = glfwCreateWindow(screenResolution.getWidth(), screenResolution.getHeight(), "The Sugarpunky Chocolate Disaster", monitor, 0);
  glfwMakeContextCurrent(window);
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
  game = new Game(window, camera, shadowCamera, options, screenResolution);
  game->setup();

  //game loop
  std::thread inputHandlingThread([]()
    {
      while(!glfwWindowShouldClose(window))
        {
          glfwPollEvents();
          std::this_thread::yield();
        }
    });
  while(!glfwWindowShouldClose(window))
    {
      game->loop();
    }
  inputHandlingThread.join();

  //cleanup
  delete game;
  glfwDestroyWindow(window);
  glfwTerminate();
}
