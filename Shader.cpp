#include "Shader.h"

Shader::Shader(const std::__cxx11::string &vertexFile, const std::__cxx11::string &fragmentFile)
{
  GLuint vertex = loadShader(GL_VERTEX_SHADER, vertexFile);
  GLuint fragment = loadShader(GL_FRAGMENT_SHADER, fragmentFile);
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  glGetProgramiv(ID, GL_LINK_STATUS, &status);
  if (status != 1)
    {
      glGetProgramInfoLog(ID, 512, NULL, infoLog);
      std::cout << infoLog << std::endl;
    }
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::setInt(const std::__cxx11::string &uniformName, int value)
{
  glUniform1i(glGetUniformLocation(ID, uniformName.c_str()), value);
}

void Shader::setFloat(const std::__cxx11::string &uniformName, float value)
{
  glUniform1f(glGetUniformLocation(ID, uniformName.c_str()), value);
}

void Shader::setBool(const std::__cxx11::string &uniformName, bool value)
{
  glUniform1i(glGetUniformLocation(ID, uniformName.c_str()), value);
}

void Shader::setVec3(const std::__cxx11::string &uniformName, glm::vec3 vec)
{
  setVec3(uniformName, vec.x, vec.y, vec.z);
}

void Shader::setVec3(const std::__cxx11::string &uniformName, float x, float y, float z)
{
  glUniform3f(glGetUniformLocation(ID, uniformName.c_str()), x, y, z);
}

void Shader::setVec2(const std::__cxx11::string &uniformName, glm::vec2 vec)
{
  setVec2(uniformName, vec.x, vec.y);
}

void Shader::setVec2(const std::__cxx11::string &uniformName, float x, float y)
{
  glUniform2f(glGetUniformLocation(ID, uniformName.c_str()), x, y);
}

void Shader::setMat4(const std::__cxx11::string &uniformName, glm::mat4 mat)
{
  glUniformMatrix4fv(glGetUniformLocation(ID, uniformName.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

GLuint Shader::getID() const
{
  return ID;
}

void Shader::use() const
{
  glUseProgram(ID);
}

void Shader::cleanUp()
{
  glDeleteProgram(ID);
}

GLuint Shader::loadShader(GLenum shaderType, const std::__cxx11::string &file)
{
  std::fstream fileStream(file);
  std::stringstream fileStringStream;
  fileStringStream << fileStream.rdbuf();
  fileStream.close();
  std::string stringSrc = fileStringStream.str();
  const char* src = stringSrc.c_str();
  GLuint shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &src, NULL);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status != 1)
    {
      glGetShaderInfoLog(shader, 512, NULL, infoLog);
      std::cout << infoLog << std::endl;
      return NULL;
    }
  return shader;
}
