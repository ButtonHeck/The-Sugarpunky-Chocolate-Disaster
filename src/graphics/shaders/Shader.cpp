/*
 * Copyright 2019 Ilya Malgin
 * Shader.cpp
 * This file is part of The Sugarpunky Chocolate Disaster project
 *
 * The Sugarpunky Chocolate Disaster project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The Sugarpunky Chocolate Disaster project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * See <http://www.gnu.org/licenses/>
 *
 * Purpose: contains definitions for Shader class
 * @version 0.1.0
 */

#include "Shader"
#include "DirectoriesSettings"
#include "Logger"

#include <sstream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

bool Shader::useCachingOfUniforms = false;

/**
* @brief sets uniforms caching mode
* @param useCache caching flag
*/
void Shader::setCachingOfUniformsMode(bool useCache) noexcept
{
  Shader::useCachingOfUniforms = useCache;
}

/**
* @brief plain ctor that creates a program consisting of only vertex shader
* @param srcFile1 source file
* @param includes list of auxiliary source files including in the shader source (might be empty)
*/
Shader::Shader(ShaderSource srcFile1, ShaderIncludeList includes)
{
  const std::string& src = srcFile1.second;
  GLuint shader = createShader(srcFile1.first, std::string(SHADER_DIR + src), includes);
  ID = glCreateProgram();
  shaderName = src.substr(0, src.find('/'));
  glAttachShader(ID, shader);
  link();
  glDeleteShader(shader);
}

/**
* @brief plain ctor that creates a program consisting of two source files (vertex+geometry or vertex+fragment)
* @param srcFile1 first source file
* @param srcFile2 second source file
* @param includes list of auxiliary source files including in the shader sources (might be empty)
*/
Shader::Shader(ShaderSource srcFile1, ShaderSource srcFile2, ShaderIncludeList includes)
{
  const std::string& src1 = srcFile1.second;
  const std::string& src2 = srcFile2.second;
  GLuint shader1 = createShader(srcFile1.first, std::string(SHADER_DIR + src1), includes);
  GLuint shader2 = createShader(srcFile2.first, std::string(SHADER_DIR + src2), includes);
  ID = glCreateProgram();
  shaderName = src1.substr(0, src1.find('/'));
  glAttachShader(ID, shader1);
  glAttachShader(ID, shader2);
  link();
  glDeleteShader(shader1);
  glDeleteShader(shader2);
}

/**
* @brief plain ctor that creates a program consisting of three source files
* @param srcFile1 first source file
* @param srcFile2 second source file
* @param srcFile3 third source file
* @param includes list of auxiliary source files including in the shader sources (might be empty)
*/
Shader::Shader(ShaderSource srcFile1, ShaderSource srcFile2, ShaderSource srcFile3, ShaderIncludeList includes)
{
  const std::string& src1 = srcFile1.second;
  const std::string& src2 = srcFile2.second;
  const std::string& src3 = srcFile3.second;
  GLuint shader1 = createShader(srcFile1.first, std::string(SHADER_DIR + src1), includes);
  GLuint shader2 = createShader(srcFile2.first, std::string(SHADER_DIR + src2), includes);
  GLuint shader3 = createShader(srcFile3.first, std::string(SHADER_DIR + src3), includes);
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

/**
* @brief sends link command to OpenGL
*/
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

GLuint Shader::getID() const noexcept
{
  return ID;
}

/**
* @brief sends use program command to OpenGL
*/
void Shader::use() const noexcept
{
  glUseProgram(ID);
}

/**
* @brief returns GL defined uniform location for a uniform
* @param uniformName name of the uniform in program
*/
GLuint Shader::getUniformLocation(const char* uniformName) const
{
  auto uniformLocation = glGetUniformLocation(ID, uniformName);
  if (uniformLocation == -1)
    Logger::log("Unknown uniform: % for: %\n", uniformName, shaderName);
  return uniformLocation;
}

/**
* @brief updates int uniform
* @param uniformName name of the uniform in program
* @param value value
*/
void Shader::setInt(const char* uniformName, int value)
{
  if (Shader::useCachingOfUniforms)
    {
      if (uniformCache.find(uniformName) == uniformCache.end())
        uniformCache[uniformName] = getUniformLocation(uniformName);
      glUniform1i(uniformCache[uniformName], value);
    }
  else
    glUniform1i(getUniformLocation(uniformName), value);
}

/**
* @brief updates uint64_t uniform (mainly used as bindless texture handler uniform)
* @param uniformName name of the uniform in program
* @param value value
*/
void Shader::setUint64(const char* uniformName, GLuint64 value)
{
  if (Shader::useCachingOfUniforms)
    {
      if (uniformCache.find(uniformName) == uniformCache.end())
        uniformCache[uniformName] = getUniformLocation(uniformName);
      glUniform1ui64ARB(uniformCache[uniformName], value);
    }
  else
    glUniform1ui64ARB(getUniformLocation(uniformName), value);
}

/**
* @brief updates float uniform
* @param uniformName name of the uniform in program
* @param value value
*/
void Shader::setFloat(const char* uniformName, float value)
{
  if (Shader::useCachingOfUniforms)
    {
      if (uniformCache.find(uniformName) == uniformCache.end())
        uniformCache[uniformName] = getUniformLocation(uniformName);
      glUniform1f(uniformCache[uniformName], value);
    }
  else
    glUniform1f(getUniformLocation(uniformName), value);
}

/**
* @brief updates int uniform that are treated as bool flags
* @param uniformName name of the uniform in program
* @param value value
*/
void Shader::setBool(const char* uniformName, bool value)
{
  if (Shader::useCachingOfUniforms)
    {
      if (uniformCache.find(uniformName) == uniformCache.end())
        uniformCache[uniformName] = getUniformLocation(uniformName);
      glUniform1i(uniformCache[uniformName], value);
    }
  else
    glUniform1i(getUniformLocation(uniformName), value);
}

/**
* @brief updates vec3 uniform
* @param uniformName name of the uniform in program
*/
void Shader::setVec3(const char* uniformName, float x, float y, float z)
{
  if (Shader::useCachingOfUniforms)
    {
      if (uniformCache.find(uniformName) == uniformCache.end())
        uniformCache[uniformName] = getUniformLocation(uniformName);
      glUniform3f(uniformCache[uniformName], x, y, z);
    }
  else
    glUniform3f(getUniformLocation(uniformName), x, y ,z);
}

/**
* @brief updates vec2 uniform
* @param uniformName name of the uniform in program
*/
void Shader::setVec2(const char* uniformName, float x, float y)
{
  if (Shader::useCachingOfUniforms)
    {
      if (uniformCache.find(uniformName) == uniformCache.end())
        uniformCache[uniformName] = getUniformLocation(uniformName);
      glUniform2f(uniformCache[uniformName], x, y);
    }
  else
    glUniform2f(getUniformLocation(uniformName), x, y);
}

/**
* @brief updates vec4 uniform
* @param uniformName name of the uniform in program
*/
void Shader::setVec4(const char* uniformName, float x, float y, float z, float w)
{
  if (Shader::useCachingOfUniforms)
    {
      if (uniformCache.find(uniformName) == uniformCache.end())
        uniformCache[uniformName] = getUniformLocation(uniformName);
      glUniform4f(uniformCache[uniformName], x, y, z, w);
    }
  else
    glUniform4f(getUniformLocation(uniformName), x, y ,z, w);
}

/**
* @brief updates mat3 uniform
* @param uniformName name of the uniform in program
* @param mat 3x3 matrix
*/
void Shader::setMat3(const char* uniformName, const glm::mat3 &mat)
{
  if (Shader::useCachingOfUniforms)
    {
      if (uniformCache.find(uniformName) == uniformCache.end())
        uniformCache[uniformName] = getUniformLocation(uniformName);
      glUniformMatrix3fv(uniformCache[uniformName], 1, GL_FALSE, glm::value_ptr(mat));
    }
  else
    glUniformMatrix3fv(getUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(mat));
}

/**
* @brief updates mat4 uniform
* @param uniformName name of the uniform in program
* @param mat 4x4 matrix
*/
void Shader::setMat4(const char* uniformName, const glm::mat4 &mat)
{
  if (Shader::useCachingOfUniforms)
    {
      if (uniformCache.find(uniformName) == uniformCache.end())
        uniformCache[uniformName] = getUniformLocation(uniformName);
      glUniformMatrix4fv(uniformCache[uniformName], 1, GL_FALSE, glm::value_ptr(mat));
    }
  else
    glUniformMatrix4fv(getUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(mat));
}

/**
* @brief sends delete program command to OpenGL
*/
void Shader::cleanUp() noexcept
{
  glDeleteProgram(ID);
}

/**
* @brief load source text from file, preprocess source code if necessary and send compile command to OpenGL
* @param shaderType GL defined type of a shader
* @param filename shader source file name
* @param includes list of auxiliary source files including in the shader source (might be empty)
*/
GLuint Shader::createShader(GLenum shaderType, const std::string &filename, ShaderIncludeList includes)
{
  std::fstream fileStream(filename);
  std::stringstream fileStringStream;
  fileStringStream << fileStream.rdbuf();
  fileStream.close();
  std::string stringSrc = fileStringStream.str();

  //inject pragma into source file for debug build
#ifdef _DEBUG
  std::string debugPragmaString = "#pragma debug(on)\n";
  stringSrc.insert(13, debugPragmaString);
#endif

  //inject source code from included files
  try
  {
    parseIncludes(shaderType, stringSrc, includes);
  }
  catch(std::invalid_argument& e)
  {
    Logger::log("% in file: %\n", e.what(), filename);
    throw;
  }

  /*
   * inject macro in source file if it has some HDR stuff (which is reflected in filename) 
   * and if HDR mode is enabled in the game
   */
  if (filename.find_first_of("_hdr") != std::string::npos && HDR_ENABLED)
    regexReplace(stringSrc, "#version 450\n", "#version 450\n#define HDR_ENABLED\n");

  //after all preprocessing source code is ready to be loaded to OpenGL
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

/**
* @brief preprocess source code according to include sources list
* @param shaderType GL defined type of a shader
* @param stringSrc source code of a shader
* @param includes list of auxiliary source files including in the shader source (might be empty)
*/
void Shader::parseIncludes(GLenum shaderType, std::string &stringSrc, ShaderIncludeList includes)
{
  for (auto i = includes.begin(); i < includes.end(); i++)
    {
	  //bypass file from include list if its shader type is different
      if (i->first != shaderType)
        continue;

      std::string includeFileName = i->second;
      std::fstream includeStream(std::string(SHADER_DIR + "include/" + i->second));
      std::stringstream includeStringStream;
      includeStringStream << includeStream.rdbuf();
      includeStream.close();
      std::string includeSrc = includeStringStream.str();

	  //replace custom lines of code in a source with the source from included file
      std::string includeToken = std::string("@include ").append(includeFileName);
      regexReplace(stringSrc, includeToken, includeSrc);
    }

  //if there are any custom lines left in the source code we must have fucked up with include list
  if (stringSrc.find("@include") != std::string::npos)
    throw std::invalid_argument("Some expected includes were not found");
}

/**
* @brief custom wrapper function for std::regex_replace whose header hugely increases output object file size
* @param source source code of a shader
* @param toReplace string to be replaced in source code
* @param substitution source code to be injected
* @note purposely replaces only one occurence of custom line per function call
*/
void Shader::regexReplace(std::string &source, const std::string &toReplace, const std::string &substitution)
{
  size_t positionOfReplaceOccurence = source.find(toReplace);
  if(positionOfReplaceOccurence != std::string::npos)
    source.replace(positionOfReplaceOccurence, toReplace.size(), substitution);
}
