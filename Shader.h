#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
  Shader(const std::string& vertexFile, const std::string& fragmentFile);
  Shader(const std::string& vertexFile, const std::string& geometryFile, const std::string& fragmentFile);
  void setInt(const std::string& uniformName, int value);
  void setFloat(const std::string& uniformName, float value);
  void setBool(const std::string& uniformName, bool value);
  void setVec3(const std::string& uniformName, glm::vec3 vec);
  void setVec3(const std::string& uniformName, float x, float y, float z);
  void setVec2(const std::string& uniformName, glm::vec2 vec);
  void setVec2(const std::string& uniformName, float x, float y);
  void setMat4(const std::string& uniformName, glm::mat4 mat);
  GLuint getID() const;
  void use() const;
  void cleanUp();
private:
  GLuint loadShader(GLenum shaderType, const std::string& file);
  GLuint ID;
  int status;
  char infoLog[512];
};

#endif // SHADERPROGRAM_H
