/*
 * Copyright 2019 Ilya Malgin
 * BuildableRenderer.cpp
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
 * Purpose: contains definition for BuildableRenderer class
 * @version 0.1.0
 */

#include "BuildableRenderer"
#include "BuildableGenerator"

/**
* @brief plain ctor
* @param generator generator that holds necessary GL objects pipelines
*/
BuildableRenderer::BuildableRenderer(BuildableGenerator &generator) noexcept
  :
    generator(generator)
{}

/**
* @brief sends draw command to OpenGL for buildable tiles instances
*/
void BuildableRenderer::renderBuildable()
{
  generator.basicGLBuffers.bind(VAO);
  glEnable(GL_BLEND);
  glDrawElementsInstanced(GL_TRIANGLES, VERTICES_PER_QUAD, GL_UNSIGNED_BYTE, 0, generator.tiles.size());
  glDisable(GL_BLEND);
}

/**
* @brief sends draw command to OpenGL for selected tile
*/
void BuildableRenderer::renderSelected()
{
  generator.selectedBuffers.bind(VAO);
  glEnable(GL_BLEND);
  glDrawElements(GL_TRIANGLES, VERTICES_PER_QUAD, GL_UNSIGNED_BYTE, 0);
  glDisable(GL_BLEND);
}
