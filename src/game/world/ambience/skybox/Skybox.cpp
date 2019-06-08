/*
 * Copyright 2019 Ilya Malgin
 * Skybox.cpp
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
 * Purpose: contains definitions for Skybox class
 * @version 0.1.0
 */

#include "Skybox"

Skybox::Skybox()
  :
    basicGLBuffers(VAO | VBO)
{
  basicGLBuffers.bind(VAO | VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  BufferCollection::bindZero(VAO | VBO);
}

void Skybox::draw()
{
  basicGLBuffers.bind(VAO);
  glDrawArrays(GL_TRIANGLES, 0, VERTICES_PER_SKYBOX);
}
