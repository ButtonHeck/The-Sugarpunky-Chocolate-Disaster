/*
 * Copyright 2019 Ilya Malgin
 * TheSun.cpp
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
 * Purpose: contains definitions for TheSun class
 * @version 0.1.0
 */

#include "TheSun"
#include "SceneSettings"

/**
 * @brief initialize member variables and setup array buffer
 */
TheSun::TheSun() noexcept
  :
    ROTATION_VECTOR(0.0f, 0.0f, 1.0f),
    basicGLBuffers(VAO | VBO),
    currentPosition(HALF_WORLD_WIDTH_F, 0.0f, 0.0f)
{
  basicGLBuffers.bind(VAO | VBO);
  GLfloat vertices[4] = {1.0f, 0.0f, 0.0f, 1.0f};
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  BufferCollection::bindZero(VAO | VBO);
}

/**
 * @brief calculates new position, light direction and updates transformation matrix
 * @param angleDegrees angle to rotate in degrees
 */
void TheSun::move(float angleDegrees)
{
  currentPosition = glm::rotate(currentPosition, glm::radians(angleDegrees), ROTATION_VECTOR);
  rotationTransform = glm::rotate(rotationTransform, glm::radians(angleDegrees), ROTATION_VECTOR);
  lightDirTo = glm::normalize(-currentPosition);
}

/**
 * @brief sets position, light direction and transformation matrix depending on given angle
 * @param angleDegrees angle to set in degrees
 * @todo TEST ONLY! remove this from the game release version
 */
void TheSun::moveAbsolutePosition(float angleDegrees)
{
  currentPosition = glm::rotate(START_POSITION, glm::radians(angleDegrees), ROTATION_VECTOR);
  rotationTransform = glm::rotate(START_MODEL, glm::radians(angleDegrees), ROTATION_VECTOR);
  lightDirTo = glm::normalize(-currentPosition);
}

const glm::vec3& TheSun::getPosition() const noexcept
{
  return currentPosition;
}

const glm::vec3 &TheSun::getLightDir() const noexcept
{
  return lightDirTo;
}

const glm::mat4 &TheSun::getRotationTransform() const noexcept
{
  return rotationTransform;
}
