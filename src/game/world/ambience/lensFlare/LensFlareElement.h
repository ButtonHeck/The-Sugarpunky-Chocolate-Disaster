/*
 * Copyright 2019 Ilya Malgin
 * LensFlareElement.h
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
 * Purpose: contains declaration for LensFlareElement class
 * @version 0.1.0
 */

#pragma once

#include <glm/vec2.hpp>
#include <GL/glew.h>

/**
 * @brief This class represents a single lens flare element which is being sent to GPU as a point
 * whose size is set depending on the current screen resolution (approximately),
 * responsible for handling it's texture ID, point size and position on the screen
 */
class LensFlareElement
{
public:
  LensFlareElement(float pointSize, GLuint textureID);
  void setPosition(const glm::vec2 &position);
  const glm::vec2& getPosition() const;
  float getPointSize() const;
  GLuint getTextureID() const;
  void adjustPointSize(float pointSizeDivisor);

private:
  glm::vec2 position;
  float pointSize;
  GLuint textureID;
};
