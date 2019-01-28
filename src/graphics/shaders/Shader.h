#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H
#include <sstream>
#include <fstream>
#include <string>
#include <regex>
#include <unordered_map>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include "util/DirectoriesSettings.h"
#include "graphics/GraphicsSettings.h"
#include "util/Logger.h"

class Shader
{
public:
  using ShaderIncludeList = std::initializer_list<std::pair<GLenum, std::string>>;
  using ShaderSource = std::pair<GLenum, const std::string>;
  Shader() = default;
  Shader(ShaderSource srcFile1,
         ShaderIncludeList includes = {});
  Shader(ShaderSource srcFile1,
         ShaderSource srcFile2,
         ShaderIncludeList includes = {});
  Shader(ShaderSource srcFile1,
         ShaderSource srcFile2,
         ShaderSource srcFile3,
         ShaderIncludeList includes = {});
  void link();
  static void cacheUniformsMode(bool cache);
  GLuint getUniformLocation(const std::string& uniformName);
  void setInt(const std::string& uniformName, int value);
  void setUInt64(const std::string& uniformName, GLuint64 value);
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
  GLuint loadShader(GLenum shaderType,
                    const std::string& filename,
                    ShaderIncludeList includes);
  void parseIncludes(GLenum shaderType,
                     std::string& stringSrc,
                     ShaderIncludeList includes);
  GLuint ID;
  std::string shaderName;
  int status;
  char infoLog[512];
  std::unordered_map<std::string, GLint> uniformCache;
};

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