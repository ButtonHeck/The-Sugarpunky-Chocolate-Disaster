/*
 * Copyright 2019 Ilya Malgin
 * TextManager.h
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
 * Purpose: contains declaration for TextManager class and GlyphVertex struct
 * @version 0.1.0
 */

#pragma once

#include "FontLoader"
#include "BufferCollection"

#include <glm/vec2.hpp>

class Camera;
class ScreenResolution;
class Options;
class Shader;
class MouseInputManager;

/**
* @brief utility manager class for handling all game text related stuff: updating text on the screen, text rendering etc.
*/
class TextManager
{
public:
  TextManager(const std::string& fontFile, const std::string& fontTexture, Shader& shader, const ScreenResolution &screenResolution);
  void addDebugText(const Camera &camera,
                    Options &options,
                    const MouseInputManager& mouseInput,
                    const glm::vec3 &sunPosition,
                    unsigned int fps);
  void drawText();

private:
  constexpr static unsigned int MAX_BUFFER_SIZE = 1024 * 24;
  const glm::vec2 DEFAULT_SCALE = glm::vec2(0.19f, 0.2f);

  /**
  * @brief representation of a glyph vertex as it is in the font shader
  */
  struct GlyphVertex
  {
    constexpr static unsigned int NUMBER_OF_ELEMENTS = 4;
    GlyphVertex(glm::vec2 pos, glm::vec2 texCoords) noexcept;

	struct
	{
		float x, y;
	} position;
	struct
	{
		float x, y;
	} texCoords;
  };

  void addString(const std::string &text, GLfloat x, GLfloat y, const glm::vec2 &scale);

  FontLoader fontLoader;
  Shader& shader;
  const ScreenResolution& screenResolution;
  glm::vec2 resolutionRelativeOffset;
  glm::vec2 scale;
  BufferCollection basicGLBuffers;
  GLfloat vertexData[MAX_BUFFER_SIZE] = {0};
  int bufferOffset, glyphsCount;
};
