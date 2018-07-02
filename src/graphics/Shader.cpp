#include "src/graphics/Shader.h"

Shader::Shader(const std::string &vertexFile)
{
  GLuint vertex = loadShader(GL_VERTEX_SHADER, std::string(RES_DIR + vertexFile));
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glLinkProgram(ID);
  glGetProgramiv(ID, GL_LINK_STATUS, &status);
  if (status != 1)
    {
      glGetProgramInfoLog(ID, 512, NULL, infoLog);
      std::cout << infoLog << std::endl;
    }
  glDeleteShader(vertex);
}

Shader::Shader(const std::__cxx11::string &vertexFile, const std::__cxx11::string &fragmentFile)
{
  GLuint vertex = loadShader(GL_VERTEX_SHADER, std::string(RES_DIR + vertexFile));
  GLuint fragment = loadShader(GL_FRAGMENT_SHADER, std::string(RES_DIR + fragmentFile));
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

Shader::Shader(const std::string &vertexFile, const std::string &geometryFile, const std::string &fragmentFile)
{
  GLuint vertex = loadShader(GL_VERTEX_SHADER, std::string(RES_DIR + vertexFile));
  GLuint geometry = loadShader(GL_GEOMETRY_SHADER, std::string(RES_DIR + geometryFile));
  GLuint fragment = loadShader(GL_FRAGMENT_SHADER, std::string(RES_DIR + fragmentFile));
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, geometry);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  glGetProgramiv(ID, GL_LINK_STATUS, &status);
  if (status != 1)
    {
      glGetProgramInfoLog(ID, 512, NULL, infoLog);
      std::cout << infoLog << std::endl;
    }
  glDeleteShader(vertex);
  glDeleteShader(geometry);
  glDeleteShader(fragment);
}

void Shader::setInt(const std::__cxx11::string &uniformName, int value)
{
  if (uniformCache.find(uniformName) == uniformCache.end())
    uniformCache[uniformName.c_str()] = glGetUniformLocation(ID, uniformName.c_str());
  glUniform1i(uniformCache[uniformName.c_str()], value);
}

void Shader::setFloat(const std::__cxx11::string &uniformName, float value)
{
  if (uniformCache.find(uniformName) == uniformCache.end())
    uniformCache[uniformName.c_str()] = glGetUniformLocation(ID, uniformName.c_str());
  glUniform1f(uniformCache[uniformName.c_str()], value);
}

void Shader::setBool(const std::__cxx11::string &uniformName, bool value)
{
  if (uniformCache.find(uniformName) == uniformCache.end())
    uniformCache[uniformName.c_str()] = glGetUniformLocation(ID, uniformName.c_str());
  glUniform1i(uniformCache[uniformName.c_str()], value);
}

void Shader::setVec3(const std::__cxx11::string &uniformName, glm::vec3 vec)
{
  setVec3(uniformName, vec.x, vec.y, vec.z);
}

void Shader::setVec3(const std::__cxx11::string &uniformName, float x, float y, float z)
{
  if (uniformCache.find(uniformName) == uniformCache.end())
    uniformCache[uniformName.c_str()] = glGetUniformLocation(ID, uniformName.c_str());
  glUniform3f(uniformCache[uniformName.c_str()], x, y, z);
}

void Shader::setVec2(const std::__cxx11::string &uniformName, glm::vec2 vec)
{
  setVec2(uniformName, vec.x, vec.y);
}

void Shader::setVec2(const std::__cxx11::string &uniformName, float x, float y)
{
  if (uniformCache.find(uniformName) == uniformCache.end())
    uniformCache[uniformName.c_str()] = glGetUniformLocation(ID, uniformName.c_str());
  glUniform2f(uniformCache[uniformName.c_str()], x, y);
}

void Shader::setVec4(const std::string &uniformName, glm::vec4 vec)
{
  if (uniformCache.find(uniformName) == uniformCache.end())
    uniformCache[uniformName.c_str()] = glGetUniformLocation(ID, uniformName.c_str());
  glUniform4f(uniformCache[uniformName.c_str()], vec.x, vec.y, vec.z, vec.w);
}

void Shader::setMat4(const std::__cxx11::string &uniformName, glm::mat4 mat)
{
  if (uniformCache.find(uniformName) == uniformCache.end())
    uniformCache[uniformName.c_str()] = glGetUniformLocation(ID, uniformName.c_str());
  glUniformMatrix4fv(uniformCache[uniformName.c_str()], 1, GL_FALSE, glm::value_ptr(mat));
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
