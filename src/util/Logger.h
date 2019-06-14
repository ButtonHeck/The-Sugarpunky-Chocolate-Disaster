#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

namespace Logger
{
  void log(const char* msg);

  template <typename T, typename... Args>
  void log(const char* pattern, T token, Args... rest)
  {
    std::string message = std::string(pattern).append(token);
	for (; *pattern != '\0'; pattern++) {
		if (*pattern == '%') {
			std::cout << token;
			log(pattern + 1, rest...);
			return;
		}
		std::cout << *pattern;
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
