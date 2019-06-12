#include "Game"
#include "ScreenResolution"
#include "Logger"
#include "Camera"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

ScreenResolution screenResolution;
Camera camera(glm::vec3(0.0f, 12.0f, 0.0f));
Camera shadowCamera(camera);

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
  GLFWwindow* window = glfwCreateWindow(screenResolution.getWidth(), screenResolution.getHeight(), "The Sugarpunky Chocolate Disaster", monitor, 0);
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

  glfwMakeContextCurrent(NULL);

  std::thread gameThread([&]()
  {
	  glfwMakeContextCurrent(window);
	  Game* game = new Game(window, camera, shadowCamera, screenResolution);
	  game->setup();
	  while (!glfwWindowShouldClose(window))
	  {
		  game->loop();
	  }
	  delete game;
  });

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
