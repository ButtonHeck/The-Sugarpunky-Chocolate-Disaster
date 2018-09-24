#include <iostream>
#include <chrono>
#include <thread>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <unordered_set>
#include "game/Game.h"
#include "game/Options.h"
#include "game/Settings.h"
#include "game/ScreenResolution.h"
#include "graphics/Camera.h"
#include "timer/BenchmarkTimer.h"

std::unordered_set<GLuint> debugMessages;
void APIENTRY glDebugCallback(GLenum source,
                            GLenum type,
                            GLuint id,
                            GLenum severity,
                            GLsizei,
                            const GLchar* glMessage,
                            const void*)
{
  if (id == 131169 || id == 131185 || id == 131218 || id == 131204 || debugMessages.find(id) != debugMessages.end()) return;
  debugMessages.emplace(id);
  std::string message = "Debug message: (" + std::to_string(id) + "), ";
  switch(source)
    {
    case GL_DEBUG_SOURCE_API:             message.append("Source: API, "); break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   message.append("Source: Window System, "); break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: message.append("Source: Shader Compiler, "); break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     message.append("Source: Third Party, "); break;
    case GL_DEBUG_SOURCE_APPLICATION:     message.append("Source: Application, "); break;
    case GL_DEBUG_SOURCE_OTHER:           message.append("Source: Other, "); break;
    }
  switch(type)
    {
    case GL_DEBUG_TYPE_ERROR:               message.append("Type: Error, "); break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: message.append("Type: Deprecated behaviour, "); break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  message.append("Type: Undefined behaviour, "); break;
    case GL_DEBUG_TYPE_PORTABILITY:         message.append("Type: Portability, "); break;
    case GL_DEBUG_TYPE_PERFORMANCE:         message.append("Type: Performance, "); break;
    case GL_DEBUG_TYPE_MARKER:              message.append("Type: Marker, "); break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          message.append("Type: Push Group, "); break;
    case GL_DEBUG_TYPE_POP_GROUP:           message.append("Type: Pop Group, "); break;
    case GL_DEBUG_TYPE_OTHER:               message.append("Type: Other, "); break;
    }
  switch(severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:          message.append("Severity: High"); break;
    case GL_DEBUG_SEVERITY_MEDIUM:        message.append("Severity: Medium"); break;
    case GL_DEBUG_SEVERITY_LOW:           message.append("Severity: Low"); break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:  message.append("Severity: Notification"); break;
    }
  std::cout << message << '\n' << glMessage << "\n\n";
}

GLFWwindow* window;
ScreenResolution screenResolution;
Camera camera(glm::vec3(0.0f, 12.0f, 0.0f));
glm::vec3 cursorToViewportDirection;
Options options;
Game* game;

int main()
{
  //initializing and presetup routines
  glfwSetErrorCallback([](int, const char* msg){printf("Error with GLFW: %s", msg);});
  if (!glfwInit())
    throw std::runtime_error("Error while loading GLFW\n");
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef _DEBUG
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  std::cout << glfwGetVersionString() << std::endl;
#endif
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
  screenResolution.updateResolution(vidmode->width, vidmode->height);
  window = glfwCreateWindow(screenResolution.getWidth(), screenResolution.getHeight(), "The Sugarpunky Chocolate Disaster", monitor, 0);
  glfwMakeContextCurrent(window);
  glewExperimental = GL_TRUE;
  glewInit();
#ifdef _DEBUG
  GLint flags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
      glEnable(GL_DEBUG_OUTPUT);
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glDebugMessageCallback(glDebugCallback, nullptr);
      glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
#endif
  game = new Game(window, cursorToViewportDirection, camera, options, screenResolution);
  game->setupVariables();

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
