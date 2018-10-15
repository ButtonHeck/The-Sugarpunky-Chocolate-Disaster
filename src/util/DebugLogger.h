#ifndef ERRORLOGGER_H
#define ERRORLOGGER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iosfwd>
#include <string>
#include <unordered_set>

namespace DebugLogger
{
  void setupWindowLibraryErrorCallback();
  void APIENTRY glDebugCallback(GLenum source,
                              GLenum type,
                              GLuint id,
                              GLenum severity,
                              GLsizei,
                              const GLchar* glMessage,
                              const void*);
};

#endif // ERRORLOGGER_H
