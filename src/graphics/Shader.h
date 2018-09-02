#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <unordered_map>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include "game/Settings.h"

class Shader
{
public:
  Shader(const std::string& vertexFile);
  Shader(const std::string& vertexFile, const std::string& fragmentFile);
  Shader(const std::string& vertexFile, const std::string& geometryFile, const std::string& fragmentFile);
  void linkAgain();
  static void cacheUniformsMode(bool cache);
  GLuint getUniformLocation(const std::string& uniformName);
  void setInt(const std::string& uniformName, int value);
  void setFloat(const std::string& uniformName, float value);
  void setBool(const std::string& uniformName, bool value);
  void setVec3(const std::string& uniformName, glm::vec3 vec);
  void setVec3(const std::string& uniformName, float x, float y, float z);
  void setVec2(const std::string& uniformName, glm::vec2 vec);
  void setVec2(const std::string& uniformName, float x, float y);
  void setVec4(const std::string& uniformName, glm::vec4 vec);
  void setVec4(const std::string& uniformName, float x, float y, float z, float w);
  void setMat3(const std::string& uniformName, glm::mat3 mat);
  void setMat4(const std::string& uniformName, glm::mat4 mat);
  GLuint getID() const;
  void use() const;
  void cleanUp();
private:
  static bool cachedUniforms;
  GLuint loadShader(GLenum shaderType, const std::string& filename);
  GLuint ID;
  int status;
  char infoLog[512];
  std::unordered_map<std::string, GLint> uniformCache;
};

inline GLuint Shader::getUniformLocation(const std::string &uniformName)
{
  auto uniformLocation = glGetUniformLocation(ID, uniformName.c_str());
  if (uniformLocation == -1)
    std::cerr << "Unknown uniform: " << uniformName.c_str() << " for ID: " << ID << '\n';
  return uniformLocation;
}

inline void Shader::setVec4(const std::__cxx11::string &uniformName, glm::vec4 vec)
{
  setVec4(uniformName, vec.x, vec.y, vec.z, vec.w);
}

inline void Shader::setVec3(const std::__cxx11::string &uniformName, glm::vec3 vec)
{
  setVec3(uniformName, vec.x, vec.y, vec.z);
}

inline void Shader::setVec2(const std::__cxx11::string &uniformName, glm::vec2 vec)
{
  setVec2(uniformName, vec.x, vec.y);
}

#endif // SHADERPROGRAM_H
