#ifndef ERRORLOGGER_H
#define ERRORLOGGER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <unordered_set>

class DebugLogger
{
public:
  DebugLogger() = default;
  static void setupWindowLibraryErrorCallback();
  static void APIENTRY glDebugCallback(GLenum source,
                              GLenum type,
                              GLuint id,
                              GLenum severity,
                              GLsizei,
                              const GLchar* glMessage,
                              const void*);
};

#endif // ERRORLOGGER_H
