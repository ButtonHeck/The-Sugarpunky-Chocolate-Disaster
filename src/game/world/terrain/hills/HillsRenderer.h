/*
 * Copyright 2019 Ilya Malgin
 * HillsRenderer.h
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
 * Purpose: contains declaration for HillsRenderer class
 * @version 0.1.0
 */

#pragma once

#include <GL/glew.h>
#include <glm/vec2.hpp>

class HillsGenerator;
class HillsShader;

/**
* @brief Renderer for hills. Responsible both for onscreen and offscreen (for frustum culling) rendering
*/
class HillsRenderer
{
public:
  HillsRenderer(HillsShader& shaders, HillsGenerator& generator) noexcept;
  void render(bool useFrustumCulling, const glm::vec2 &viewAcceleration);
  void renderDepthmap();
  void debugRender(GLenum primitiveType);

private:
  HillsShader& shaders;
  HillsGenerator& generator;
};
