/*
 * Copyright 2019 Ilya Malgin
 * LensFlareElement.cpp
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
 * Purpose: contains definitions for LensFlareElement class
 * @version 0.1.0
 */

#include "LensFlareElement.h"

LensFlareElement::LensFlareElement(float pointSize, GLuint textureID)
  :
    pointSize(pointSize),
    textureID(textureID)
{}

void LensFlareElement::setPosition(const glm::vec2 &position)
{
  this->position = position;
}

const glm::vec2 &LensFlareElement::getPosition() const
{
  return position;
}

float LensFlareElement::getPointSize() const
{
  return pointSize;
}

GLuint LensFlareElement::getTextureID() const
{
  return textureID;
}

/**
 * @brief multiplies flare's current point size with a given value
 * @param pointSizeDivisor multiplier for flare's point size
 * (might be greater than 1.0 if current screen resolution is greater than the reference
 * or lesser than 1.0 otherwise)
 */
void LensFlareElement::adjustPointSize(float pointSizeDivisor)
{
  pointSize *= pointSizeDivisor;
}
