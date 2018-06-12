#include <iostream>
#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "src/game/Game.h"
#include "src/game/Settings.h"
#include "src/game/Options.h"
#include "src/graphics/Camera.h"

void APIENTRY glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei, const GLchar* message, const void*);

int scr_width;
int scr_height;
float aspect_ratio;
GLFWwindow* window;
Camera camera(glm::vec3(0.0f, 3.0f, 0.0f));
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
  glfwWindowHint(GLFW_SAMPLES, 8);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
  std::cout << glfwGetVersionString() << std::endl;
  scr_width = vidmode->width;
  scr_height = vidmode->height;
  aspect_ratio = (float)scr_width / (float)scr_height;
  window = glfwCreateWindow(scr_width, scr_height, "Terrain Test", monitor, 0);
  glfwMakeContextCurrent(window);
  glewExperimental = GL_TRUE;
  glewInit();
  GLint flags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
      glEnable(GL_DEBUG_OUTPUT);
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glDebugMessageCallback(glDebugCallback, nullptr);
      glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

  game = new Game(window, cursorToViewportDirection, camera, options, scr_width, scr_height, aspect_ratio);
  game->setupVariables();

  //MAIN LOOP
  while(!glfwWindowShouldClose(window))
    {
      game->loop();
    }

  //cleanup
  delete game;
  glfwDestroyWindow(window);
  glfwTerminate();
}

void APIENTRY glDebugCallback(GLenum source,
                            GLenum type,
                            GLuint id,
                            GLenum severity,
                            GLsizei,
                            const GLchar* message,
                            const void*)
{
  if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
  std::cout << "Debug message: (" << id << "): " << message << std::endl;
  switch(source)
    {
    case GL_DEBUG_SOURCE_API: std::cout << "Source: API\n"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: std::cout << "Source: Window System\n"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler\n"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY: std::cout << "Source: Third Party\n"; break;
    case GL_DEBUG_SOURCE_APPLICATION: std::cout << "Source: Application\n"; break;
    case GL_DEBUG_SOURCE_OTHER: std::cout << "Source: Other\n"; break;
    }
  switch(type)
    {
    case GL_DEBUG_TYPE_ERROR: std::cout << "Type: Error\n"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated behaviour\n"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: std::cout << "Type: Undefined behaviour\n"; break;
    case GL_DEBUG_TYPE_PORTABILITY: std::cout << "Type: Portability\n"; break;
    case GL_DEBUG_TYPE_PERFORMANCE: std::cout << "Type: Performance\n"; break;
    case GL_DEBUG_TYPE_MARKER: std::cout << "Type: Marker\n"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP: std::cout << "Type: Push Group\n"; break;
    case GL_DEBUG_TYPE_POP_GROUP: std::cout << "Type: Pop Group\n"; break;
    case GL_DEBUG_TYPE_OTHER: std::cout << "Type: Other\n"; break;
    }
  switch(severity)
    {
    case GL_DEBUG_SEVERITY_HIGH: std::cout << "Severity: High\n\n"; break;
    case GL_DEBUG_SEVERITY_MEDIUM: std::cout << "Severity: Medium\n\n"; break;
    case GL_DEBUG_SEVERITY_LOW: std::cout << "Severity: Low\n\n"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: Notification\n\n"; break;
    }
}
