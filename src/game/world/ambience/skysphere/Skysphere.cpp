/*
 * Copyright 2019 Ilya Malgin
 * Skysphere.cpp
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
 * Purpose: contains definitions for Skysphere class
 * @version 0.1.0
 */

#include "game/world/ambience/skysphere/Skysphere.h"

/**
 * @param path path to the .obj model file
 * @param initialTransform initial rotation transform
 */
Skysphere::Skysphere(const std::string &path, const glm::mat4 &initialTransform)
  :
    model(path, false),
    modelRotationTransform(initialTransform)
{}

/**
 * @brief updates rotation tranformation
 * @param angleDegrees angle to rotate in degrees
 * @param rotationVector vector around which to rotate
 */
void Skysphere::rotate(float angleDegrees, const glm::vec3 &rotationVector)
{
  modelRotationTransform = glm::rotate(modelRotationTransform, glm::radians(angleDegrees), rotationVector);
}

/**
 * @brief delegates actual rendering to underlying model object
 */
void Skysphere::draw()
{
  model.drawOneInstance();
}

const glm::mat4 &Skysphere::getRotationTransform() const
{
  return modelRotationTransform;
}
