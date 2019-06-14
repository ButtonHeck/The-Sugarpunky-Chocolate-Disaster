#include "Logger"

#include <unordered_set>

namespace Logger
{
  void APIENTRY glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei, const GLchar *glMessage, const void *)
  {
    static std::unordered_set<GLuint> debugMessages;
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
    std::cout << message << std::endl << glMessage << std::endl << std::endl;
  }

  void log(const char *msg)
  {
    std::cout << msg;
  }
}


