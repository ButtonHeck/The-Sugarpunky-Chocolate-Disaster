#include "graphics/Shader.h"

Shader::Shader(const std::string &vertexFile)
{
  GLuint vertex = loadShader(GL_VERTEX_SHADER, std::string(RES_DIR + vertexFile), false);
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

Shader::Shader(const std::__cxx11::string &vertexFile, const std::__cxx11::string &fragmentFile, bool renameFragmentShaderVariables)
{
  GLuint vertex = loadShader(GL_VERTEX_SHADER, std::string(RES_DIR + vertexFile), false);
  GLuint fragment = loadShader(GL_FRAGMENT_SHADER, std::string(RES_DIR + fragmentFile), renameFragmentShaderVariables);
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
  GLuint vertex = loadShader(GL_VERTEX_SHADER, std::string(RES_DIR + vertexFile), false);
  GLuint geometry = loadShader(GL_GEOMETRY_SHADER, std::string(RES_DIR + geometryFile), false);
  GLuint fragment = loadShader(GL_FRAGMENT_SHADER, std::string(RES_DIR + fragmentFile), false);
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

void Shader::setVec3(const std::__cxx11::string &uniformName, float x, float y, float z)
{
  if (uniformCache.find(uniformName) == uniformCache.end())
    uniformCache[uniformName.c_str()] = glGetUniformLocation(ID, uniformName.c_str());
  glUniform3f(uniformCache[uniformName.c_str()], x, y, z);
}

void Shader::setVec2(const std::__cxx11::string &uniformName, float x, float y)
{
  if (uniformCache.find(uniformName) == uniformCache.end())
    uniformCache[uniformName.c_str()] = glGetUniformLocation(ID, uniformName.c_str());
  glUniform2f(uniformCache[uniformName.c_str()], x, y);
}

void Shader::setVec4(const std::string &uniformName, float x, float y, float z, float w)
{
  if (uniformCache.find(uniformName) == uniformCache.end())
    uniformCache[uniformName.c_str()] = glGetUniformLocation(ID, uniformName.c_str());
  glUniform4f(uniformCache[uniformName.c_str()], x, y, z, w);
}

void Shader::setMat3(const std::string &uniformName, glm::mat3 mat)
{
  if (uniformCache.find(uniformName) == uniformCache.end())
    uniformCache[uniformName.c_str()] = glGetUniformLocation(ID, uniformName.c_str());
  glUniformMatrix3fv(uniformCache[uniformName.c_str()], 1, GL_FALSE, glm::value_ptr(mat));
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

GLuint Shader::loadShader(GLenum shaderType, const std::__cxx11::string &filename, bool renameFragmentShaderVariables)
{
  std::fstream fileStream(filename);
  std::stringstream fileStringStream;
  fileStringStream << fileStream.rdbuf();
  fileStream.close();
  std::string stringSrc = fileStringStream.str();
  if (renameFragmentShaderVariables)
    stringSrc = std::regex_replace(stringSrc, std::regex("vg_"), "v_");
#ifdef _DEBUG
  std::string debugPragmaString = "#pragma debug(on)\n";
  stringSrc.insert(13, debugPragmaString);
#endif
  if (filename.find_first_of("_hdr") != std::string::npos && HDR_ENABLED)
    stringSrc = std::regex_replace(stringSrc, std::regex("#version 450\n"), "#version 450\n#define HDR_ENABLED\n");
  const char* src = stringSrc.c_str();
  GLuint shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &src, NULL);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status != 1)
    {
      glGetShaderInfoLog(shader, 512, NULL, infoLog);
      std::cout << infoLog << std::endl;
    }
  return shader;
}
