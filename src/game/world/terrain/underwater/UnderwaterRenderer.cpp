/*
 * Copyright 2019 Ilya Malgin
 * UnderwaterRenderer.cpp
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
 * Purpose: contains definitions for UnderwaterRenderer class
 * @version 0.1.0
 */

#include "UnderwaterRenderer"
#include "UnderwaterSurface"

/**
* @brief plain ctor
* @param surface underwater surface
*/
UnderwaterRenderer::UnderwaterRenderer(UnderwaterSurface &surface) noexcept
  :
    surface(surface)
{}

/**
* @brief sends draw call to OpenGL
*/
void UnderwaterRenderer::render()
{
  surface.basicGLBuffers.bind(VAO);
  glDrawElements(GL_TRIANGLES, VERTICES_PER_QUAD, GL_UNSIGNED_BYTE, 0);
}
