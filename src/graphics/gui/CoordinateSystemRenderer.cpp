/*
 * Copyright 2019 Ilya Malgin
 * CoordinateSystemRenderer.cpp
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
 * Purpose: contains definition for CoordinateSystemRenderer class
 * @version 0.1.0
 */

#include "CoordinateSystemRenderer"
#include "Shader"

/**
* @brief plain ctor. Creates data for coordinate system geometry and sets layout for OpenGL buffer collection
* @param shader shader program used during CS rendering routine (this one includes geometry shader)
*/
CoordinateSystemRenderer::CoordinateSystemRenderer(Shader *shader)
  :
    basicGLBuffers(VAO | VBO),
    shader(shader)
{
  basicGLBuffers.bind(VAO | VBO);
  constexpr GLfloat POINTS[] = {
      -0.92f, 0.58f, 0.0f, 0.08f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
      -0.92f, 0.58f, 0.0f, 0.0f,  0.08f, 0.0f,  0.0f, 1.0f, 0.0f,
      -0.92f, 0.58f, 0.0f, 0.0f,  0.0f,  0.08f, 0.0f, 0.0f, 1.0f
  };
  glBufferData(GL_ARRAY_BUFFER, sizeof(POINTS), POINTS, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
  BufferCollection::bindZero(VAO | VBO);
}

/**
* @brief updates shader variables and sends draw call to OpenGL
* @param view 3x3 view matrix
* @param aspectRatio aspect ratio of current viewport
*/
void CoordinateSystemRenderer::draw(const glm::mat3 &view, float aspectRatio)
{
  glLineWidth(2);
  shader->use();
  shader->setMat3("u_view", view);
  shader->setFloat("u_aspectRatio", aspectRatio);
  basicGLBuffers.bind(VAO);
  glDrawArrays(GL_POINTS, 0, 3);
  glLineWidth(1);
}
