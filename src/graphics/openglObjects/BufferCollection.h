/*
 * Copyright 2019 Ilya Malgin
 * BufferCollection.h
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
 * Purpose: contains declaration for BufferCollection class and some global constants
 * @version 0.1.0
 */

#pragma once

#include <GL/glew.h>
#include <unordered_map>

constexpr unsigned char QUAD_INDICES[6] = {0,1,2,2,3,0};
constexpr unsigned int VERTICES_PER_QUAD = 6;
constexpr unsigned int INDIRECT_DRAW_COMMAND_ARGUMENTS = 5;
constexpr GLsizeiptr INDIRECT_DRAW_COMMAND_BYTE_SIZE = sizeof(GLuint) * INDIRECT_DRAW_COMMAND_ARGUMENTS;

/**
* @brief integer flag representing each buffer object (and transform feedback)
*/
enum OPENGL_OBJECT : int
{
  VAO =           0x000001,
  VBO =           0x000010,
  INSTANCE_VBO =  0x000100,
  EBO =           0x001000,
  DIBO =          0x010000,
  TFBO =          0x100000
};

/**
* @brief a collection of OpenGL buffer objects, which are to be used as a complete buffer pipeline.
* Responsible for managing all the "glCreate/Bind/Delete" stuff and creating a collection of OpenGL objects 
* using just a set of flags representing each object's type
*/
class BufferCollection
{
public:
  BufferCollection() = default;
  explicit BufferCollection(int flags);
  BufferCollection(BufferCollection&& old) noexcept;
  BufferCollection(BufferCollection& copy);
  virtual ~BufferCollection();
  static void bindZero(int flags) noexcept;
  void create(int flags);
  void deleteBuffers();
  void deleteBuffer(int flag);
  GLuint& get(int flag);
  void bind(int flag);
  void add(int flag);

private:
  std::unordered_map<int, GLuint> objects;
};
