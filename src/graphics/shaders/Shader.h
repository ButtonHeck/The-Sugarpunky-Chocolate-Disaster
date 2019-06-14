#pragma once

#include "GraphicsSettings"

#include <unordered_map>
#include <GL/glew.h>
#include <glm/mat4x4.hpp>

class Shader
{
public:
  using ShaderIncludeList = std::initializer_list<std::pair<GLenum, std::string>>;
  using ShaderSource = std::pair<GLenum, const std::string>;
  static void cacheUniformsMode(bool cache) noexcept;

  Shader() = default;
  Shader(ShaderSource srcFile1, ShaderIncludeList includes = {});
  Shader(ShaderSource srcFile1, ShaderSource srcFile2, ShaderIncludeList includes = {});
  Shader(ShaderSource srcFile1, ShaderSource srcFile2, ShaderSource srcFile3, ShaderIncludeList includes = {});
  void link();
  GLuint getID() const noexcept;
  void use() const noexcept;
  GLuint getUniformLocation(const char* uniformName) const;
  void setInt(const char* uniformName, int value);
  void setUint64(const char* uniformName, GLuint64 value);
  void setFloat(const char* uniformName, float value);
  void setBool(const char* uniformName, bool value);
  void setVec3(const char* uniformName, const glm::vec3 &vec);
  void setVec3(const char* uniformName, float x, float y, float z);
  void setVec2(const char* uniformName, const glm::vec2 &vec);
  void setVec2(const char* uniformName, float x, float y);
  void setVec4(const char* uniformName, const glm::vec4 &vec);
  void setVec4(const char* uniformName, float x, float y, float z, float w);
  void setMat3(const char* uniformName, const glm::mat3 &mat);
  void setMat4(const char* uniformName, const glm::mat4 &mat);
  void cleanUp() noexcept;

private:
  static bool cachedUniforms;
  GLuint loadShader(GLenum shaderType, const std::string& filename, ShaderIncludeList includes);
  void parseIncludes(GLenum shaderType, std::string& stringSrc, ShaderIncludeList includes);
  void regexReplace(std::string& source, const std::string& toReplace, const std::string& substitution);

  GLuint ID;
  std::string shaderName;
  int status;
  char infoLog[512];
  std::unordered_map<const char*, GLint> uniformCache;
};

inline void Shader::setVec4(const char* uniformName, const glm::vec4 &vec)
{
  setVec4(uniformName, vec.x, vec.y, vec.z, vec.w);
}

inline void Shader::setVec3(const char* uniformName, const glm::vec3 &vec)
{
  setVec3(uniformName, vec.x, vec.y, vec.z);
}

inline void Shader::setVec2(const char* uniformName, const glm::vec2 &vec)
{
  setVec2(uniformName, vec.x, vec.y);
}
