/*
 * Copyright 2019 Ilya Malgin
 * PlantsShader.h
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
 * Purpose: contains declaration for PlantsShader class
 * @version 0.1.0
 */

#ifndef PLANTSSHADER_H
#define PLANTSSHADER_H
#include "graphics/shaders/Shader.h"

/**
 * @brief Personal shader manager for plants related code module.
 * Responsible for updating non-constant uniform variables
 */
class PlantsShader
{
public:
  PlantsShader(Shader& renderPhongShader, Shader& renderGouraudShader);
  void updateAllPlants(bool usePhongShading,
                       const glm::vec3 &lightDir,
                       const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
                       const glm::mat4 &projectionView,
                       const glm::vec3 &viewPosition,
                       bool useShadows,
                       bool useLandBlending);
  void updateGrassKeyframe();
  void setType(int type);
  void setLowPolyMode(bool isLowPoly);

private:
  Shader& renderPhongShader;
  Shader& renderGouraudShader;
  Shader* currentShader;
};

#endif // PLANTSSHADER_H
