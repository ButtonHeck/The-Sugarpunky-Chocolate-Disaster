#include "graphics/Shader.h"

bool Shader::cachedUniforms = false;

Shader::Shader(ShaderSource srcFile1,
               ShaderIncludeList includes)
{
  const std::string& src = srcFile1.second;
  GLuint shader = loadShader(srcFile1.first, std::string(SHADER_DIR + src), includes);
  ID = glCreateProgram();
  shaderName = src.substr(0, src.find('/'));
  glAttachShader(ID, shader);
  link();
  glDeleteShader(shader);
}

Shader::Shader(ShaderSource srcFile1,
               ShaderSource srcFile2,
               ShaderIncludeList includes)
{
  const std::string& src1 = srcFile1.second;
  const std::string& src2 = srcFile2.second;
  GLuint shader1 = loadShader(srcFile1.first, std::string(SHADER_DIR + src1), includes);
  GLuint shader2 = loadShader(srcFile2.first, std::string(SHADER_DIR + src2), includes);
  ID = glCreateProgram();
  shaderName = src1.substr(0, src1.find('/'));
  glAttachShader(ID, shader1);
  glAttachShader(ID, shader2);
  link();
  glDeleteShader(shader1);
  glDeleteShader(shader2);
}

Shader::Shader(ShaderSource srcFile1,
               ShaderSource srcFile2,
               ShaderSource srcFile3,
               ShaderIncludeList includes)
{
  const std::string& src1 = srcFile1.second;
  const std::string& src2 = srcFile2.second;
  const std::string& src3 = srcFile3.second;
  GLuint shader1 = loadShader(srcFile1.first, std::string(SHADER_DIR + src1), includes);
  GLuint shader2 = loadShader(srcFile2.first, std::string(SHADER_DIR + src2), includes);
  GLuint shader3 = loadShader(srcFile3.first, std::string(SHADER_DIR + src3), includes);
  ID = glCreateProgram();
  shaderName = src1.substr(0, src1.find('/'));
  glAttachShader(ID, shader1);
  glAttachShader(ID, shader2);
  glAttachShader(ID, shader3);
  link();
  glDeleteShader(shader1);
  glDeleteShader(shader2);
  glDeleteShader(shader3);
}

void Shader::link()
{
  glLinkProgram(ID);
  glGetProgramiv(ID, GL_LINK_STATUS, &status);
  if (status != 1)
    {
      glGetProgramInfoLog(ID, 512, NULL, infoLog);
      Logger::log("%\n", infoLog);
    }
}

void Shader::cacheUniformsMode(bool cache)
{
  Shader::cachedUniforms = cache;
}

GLuint Shader::getUniformLocation(const std::string &uniformName)
{
  auto uniformLocation = glGetUniformLocation(ID, uniformName.c_str());
  if (uniformLocation == -1)
    Logger::log("Unknown uniform: %\n", uniformName + " for: " + shaderName);
  return uniformLocation;
}

void Shader::setInt(const std::__cxx11::string &uniformName, int value)
{
  if (Shader::cachedUniforms)
    {
      if (uniformCache.find(uniformName) == uniformCache.end())
        uniformCache[uniformName.c_str()] = getUniformLocation(uniformName);
      glUniform1i(uniformCache[uniformName.c_str()], value);
    }
  else
    glUniform1i(getUniformLocation(uniformName), value);
}

void Shader::setUInt64(const std::string &uniformName, GLuint64 value)
{
  if (Shader::cachedUniforms)
    {
      if (uniformCache.find(uniformName) == uniformCache.end())
        uniformCache[uniformName.c_str()] = getUniformLocation(uniformName);
      glUniform1ui64ARB(uniformCache[uniformName.c_str()], value);
    }
  else
    glUniform1ui64ARB(getUniformLocation(uniformName), value);
}

void Shader::setFloat(const std::__cxx11::string &uniformName, float value)
{
  if (Shader::cachedUniforms)
    {
      if (uniformCache.find(uniformName) == uniformCache.end())
        uniformCache[uniformName.c_str()] = getUniformLocation(uniformName);
      glUniform1f(uniformCache[uniformName.c_str()], value);
    }
  else
    glUniform1f(getUniformLocation(uniformName), value);
}

void Shader::setBool(const std::__cxx11::string &uniformName, bool value)
{
  if (Shader::cachedUniforms)
    {
      if (uniformCache.find(uniformName) == uniformCache.end())
        uniformCache[uniformName.c_str()] = getUniformLocation(uniformName);
      glUniform1i(uniformCache[uniformName.c_str()], value);
    }
  else
    glUniform1i(getUniformLocation(uniformName), value);
}

void Shader::setVec3(const std::__cxx11::string &uniformName, float x, float y, float z)
{
  if (Shader::cachedUniforms)
    {
      if (uniformCache.find(uniformName) == uniformCache.end())
        uniformCache[uniformName.c_str()] = getUniformLocation(uniformName);
      glUniform3f(uniformCache[uniformName.c_str()], x, y, z);
    }
  else
    glUniform3f(getUniformLocation(uniformName), x, y ,z);
}

void Shader::setVec2(const std::__cxx11::string &uniformName, float x, float y)
{
  if (Shader::cachedUniforms)
    {
      if (uniformCache.find(uniformName) == uniformCache.end())
        uniformCache[uniformName.c_str()] = getUniformLocation(uniformName);
      glUniform2f(uniformCache[uniformName.c_str()], x, y);
    }
  else
    glUniform2f(getUniformLocation(uniformName), x, y);
}

void Shader::setVec4(const std::string &uniformName, float x, float y, float z, float w)
{
  if (Shader::cachedUniforms)
    {
      if (uniformCache.find(uniformName) == uniformCache.end())
        uniformCache[uniformName.c_str()] = getUniformLocation(uniformName);
      glUniform4f(uniformCache[uniformName.c_str()], x, y, z, w);
    }
  else
    glUniform4f(getUniformLocation(uniformName), x, y ,z, w);
}

void Shader::setMat3(const std::string &uniformName, glm::mat3 mat)
{
  if (Shader::cachedUniforms)
    {
      if (uniformCache.find(uniformName) == uniformCache.end())
        uniformCache[uniformName.c_str()] = getUniformLocation(uniformName);
      glUniformMatrix3fv(uniformCache[uniformName.c_str()], 1, GL_FALSE, glm::value_ptr(mat));
    }
  else
    glUniformMatrix3fv(getUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setMat4(const std::__cxx11::string &uniformName, glm::mat4 mat)
{
  if (Shader::cachedUniforms)
    {
      if (uniformCache.find(uniformName) == uniformCache.end())
        uniformCache[uniformName.c_str()] = getUniformLocation(uniformName);
      glUniformMatrix4fv(uniformCache[uniformName.c_str()], 1, GL_FALSE, glm::value_ptr(mat));
    }
  else
    glUniformMatrix4fv(getUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(mat));
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

GLuint Shader::loadShader(GLenum shaderType, const std::__cxx11::string &filename, ShaderIncludeList includes)
{
  std::fstream fileStream(filename);
  std::stringstream fileStringStream;
  fileStringStream << fileStream.rdbuf();
  fileStream.close();
  std::string stringSrc = fileStringStream.str();
#ifdef _DEBUG
  std::string debugPragmaString = "#pragma debug(on)\n";
  stringSrc.insert(13, debugPragmaString);
#endif
  try
  {
    parseIncludes(shaderType, stringSrc, includes);
  }
  catch(std::runtime_error e)
  {
    std::cerr << e.what() << " in file: " << filename << std::endl;
  }
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
      Logger::log("%\n", infoLog);
    }
  return shader;
}

void Shader::parseIncludes(GLenum shaderType,
                           std::string &stringSrc,
                           ShaderIncludeList includes)
{
  for (auto i = includes.begin(); i < includes.end(); i++)
    {
      if (i->first != shaderType)
        continue;
      std::string includeFileName = i->second;
      std::fstream includeStream(std::string(SHADER_DIR + "include/" + i->second));
      std::stringstream includeStringStream;
      includeStringStream << includeStream.rdbuf();
      includeStream.close();
      std::string includeSrc = includeStringStream.str();
      std::string includeToken = std::string("@include ").append(includeFileName);
      if (stringSrc.find(includeToken) != std::string::npos)
        stringSrc = std::regex_replace(stringSrc, std::regex(includeToken), includeSrc);
    }
  if (stringSrc.find("@include") != std::string::npos)
    throw std::invalid_argument("Some expected includes were not found");
}
