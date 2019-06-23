/*
 * Copyright 2019 Ilya Malgin
 * ShoreFacade.h
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
 * Purpose: contains declaration for ShoreFacade class
 * @version 0.1.0
 */

#pragma once

#include "ShoreGenerator"
#include "ShoreShader"
#include "ShoreRenderer"

/**
* @brief facade for shore related code. Responsible for delegating tasks to member objects
*/
class ShoreFacade
{
public:
  ShoreFacade(Shader &renderShader, Shader &normalsShader, const map2D_f& waterMap);
  void setup();
  void serialize(std::ofstream& output);
  void deserialize(std::ifstream& input);
  void draw(const glm::vec3 &lightDir,
            const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
            const glm::mat4 &projectionView,
            bool useShadows,
            bool useDebugRender,
            bool useClipDistanceReflection,
            bool useClipDistanceRefraction);
  const map2D_f& getMap() const noexcept;

private:
  ShoreShader shader;
  ShoreGenerator generator;
  ShoreRenderer renderer;
};
