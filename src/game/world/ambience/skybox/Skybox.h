/*
 * Copyright 2019 Ilya Malgin
 * Skybox.h
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
 * Purpose: contains declaration for Skybox class
 * @version 0.1.0
 */

#ifndef SKYBOX_H
#define SKYBOX_H
#include <GL/glew.h>
#include "graphics/openglObjects/BufferCollection.h"

/**
 * @brief This class represents a simple wrapper for box-like structure of vertices
 * and is responsible for drawing itself on the screen
 */
class Skybox
{
public:
  Skybox();
  void draw();

private:
  constexpr static unsigned int VERTICES_PER_SKYBOX = VERTICES_PER_QUAD * VERTICES_PER_QUAD;
  constexpr static unsigned int BUFFER_SIZE_FLOATS = 3 * VERTICES_PER_SKYBOX;

  BufferCollection basicGLBuffers;
  //offset a bit higher on the Y for aesthetic purpose
  GLfloat vertices[BUFFER_SIZE_FLOATS] = {
    -1.0f,  1.2f, -1.0f,
    -1.0f, -0.8f, -1.0f,
     1.0f, -0.8f, -1.0f,
     1.0f, -0.8f, -1.0f,
     1.0f,  1.2f, -1.0f,
    -1.0f,  1.2f, -1.0f,

    -1.0f, -0.8f,  1.0f,
    -1.0f, -0.8f, -1.0f,
    -1.0f,  1.2f, -1.0f,
    -1.0f,  1.2f, -1.0f,
    -1.0f,  1.2f,  1.0f,
    -1.0f, -0.8f,  1.0f,

     1.0f, -0.8f, -1.0f,
     1.0f, -0.8f,  1.0f,
     1.0f,  1.2f,  1.0f,
     1.0f,  1.2f,  1.0f,
     1.0f,  1.2f, -1.0f,
     1.0f, -0.8f, -1.0f,

    -1.0f, -0.8f,  1.0f,
    -1.0f,  1.2f,  1.0f,
     1.0f,  1.2f,  1.0f,
     1.0f,  1.2f,  1.0f,
     1.0f, -0.8f,  1.0f,
    -1.0f, -0.8f,  1.0f,

    -1.0f,  1.2f, -1.0f,
     1.0f,  1.2f, -1.0f,
     1.0f,  1.2f,  1.0f,
     1.0f,  1.2f,  1.0f,
    -1.0f,  1.2f,  1.0f,
    -1.0f,  1.2f, -1.0f,

    -1.0f, -0.8f, -1.0f,
    -1.0f, -0.8f,  1.0f,
     1.0f, -0.8f, -1.0f,
     1.0f, -0.8f, -1.0f,
    -1.0f, -0.8f,  1.0f,
     1.0f, -0.8f,  1.0f
  };
};

#endif // SKYBOX_H
