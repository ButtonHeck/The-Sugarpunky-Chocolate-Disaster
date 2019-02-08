#ifndef LOGGER_H
#define LOGGER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unordered_set>
#include <iostream>

namespace Logger
{
  void log(const char* msg);

  template <typename T, typename... Args>
  void log(const char* pattern, T token, Args... rest)
  {
    static std::unordered_set<std::string> messages;
    std::string message = std::string(pattern).append(token);
    if (messages.count(message) == 0)
      {
        messages.emplace(message);
        for ( ; *pattern != '\0'; pattern++ ) {
            if (*pattern == '%') {
                std::cout << token;
                log(pattern+1, rest...);
                return;
            }
            std::cout << *pattern;
        }
      }
  }
  void APIENTRY glDebugCallback(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei,
                                const GLchar* glMessage,
                                const void*);
};

#endif // LOGGER_H
