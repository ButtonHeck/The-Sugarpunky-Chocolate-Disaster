/*
 * Copyright 2019 Ilya Malgin
 * Shader.h
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
 * Purpose: contains declaration and inline functions for Shader class
 * @version 0.1.0
 */

#pragma once

#include "GraphicsSettings"

#include <unordered_map>
#include <GL/glew.h>
#include <glm/mat4x4.hpp>

/**
* @brief client representation of a compiled GL shader program.
* Responsible for creating, compiling and linking a shader program from given source files, managing uniform update calls
* @note objects of this class should be default-constructible as they're used as unordered_map values in shader manager
* @see ShaderManager
*/
class Shader
{
public:
	/** @note a pair of GL defined shader type and source code file name */
	using ShaderSource = std::pair<GLenum, const std::string>;

	/**
	* @note a list of ShaderSource objects (might be empty)
	* @see ShaderSource
	*/
	using ShaderIncludeList = std::initializer_list<std::pair<GLenum, std::string>>;

	static void setCachingOfUniformsMode( bool useCache ) noexcept;

	Shader() = default;
	Shader( ShaderSource srcFile1, 
			ShaderIncludeList includes = {} );
	Shader( ShaderSource srcFile1, 
			ShaderSource srcFile2, 
			ShaderIncludeList includes = {} );
	Shader( ShaderSource srcFile1, 
			ShaderSource srcFile2, 
			ShaderSource srcFile3, 
			ShaderIncludeList includes = {} );
	void link();
	GLuint getID() const noexcept;
	void use() const noexcept;
	GLuint getUniformLocation( const char * uniformName ) const;
	void setInt( const char * uniformName, 
				 int value );
	void setUint64( const char * uniformName, 
					GLuint64 value );
	void setFloat( const char * uniformName, 
				   float value );
	void setBool( const char * uniformName, 
				  bool value );
	void setVec3( const char * uniformName, 
				  const glm::vec3 & vec );
	void setVec3( const char * uniformName, 
				  float x, 
				  float y, 
				  float z );
	void setVec2( const char * uniformName, 
				  const glm::vec2 & vec );
	void setVec2( const char * uniformName, 
				  float x, 
				  float y );
	void setVec4( const char * uniformName, 
				  const glm::vec4 & vec );
	void setVec4( const char * uniformName, 
				  float x, 
				  float y, 
				  float z, 
				  float w );
	void setMat3( const char * uniformName, 
				  const glm::mat3 & mat );
	void setMat4( const char * uniformName, 
				  const glm::mat4 & mat );
	void cleanUp() noexcept;

private:
	static bool useCachingOfUniforms;
	GLuint createShader( GLenum shaderType, 
						 const std::string & filename, 
						 ShaderIncludeList includes );
	void parseIncludes( GLenum shaderType, 
						std::string & stringSrc, 
						ShaderIncludeList includes );
	void regexReplace( std::string & source, 
					   const std::string & toReplace, 
					   const std::string & substitution );

	GLuint ID;
	std::string shaderName;
	int status;
	char infoLog[512];
	std::unordered_map<const char *, GLint> uniformCache;
};

/**
* @brief updates vec4 uniform
* @param uniformName name of the uniform in program
* @param vec value
*/
inline void Shader::setVec4( const char * uniformName, 
							 const glm::vec4 & vec )
{
	setVec4( uniformName, vec.x, vec.y, vec.z, vec.w );
}

/**
* @brief updates vec3 uniform
* @param uniformName name of the uniform in program
* @param vec value
*/
inline void Shader::setVec3( const char * uniformName, 
							 const glm::vec3 & vec )
{
	setVec3( uniformName, vec.x, vec.y, vec.z );
}

/**
* @brief updates vec2 uniform
* @param uniformName name of the uniform in program
* @param vec value
*/
inline void Shader::setVec2( const char * uniformName, 
							 const glm::vec2 & vec )
{
	setVec2( uniformName, vec.x, vec.y );
}
