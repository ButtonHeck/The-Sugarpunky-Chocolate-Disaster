/*
 * Copyright 2019 Ilya Malgin
 * PlantsFacade.h
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
 * Purpose: contains declaration for PlantsFacade class and forward declaration for Frustum class
 * @version 0.1.0
 */

#pragma once

#include "TypeAliases"
#include "LandPlantsGenerator"
#include "GrassGenerator"
#include "HillTreesGenerator"
#include "PlantsShader"
#include "TreesRenderer"
#include "GrassRenderer"

class Frustum;

/**
 * @brief Facade for plants related code module.
 * Responsible for delegating tasks to its member objects accordingly and preparing distribution map for generators
 */
class PlantsFacade
{
public:
  PlantsFacade(Shader& renderPhongShader, Shader& renderGouraudShader) noexcept;
  void setup(const map2D_f &landMap, const map2D_f &hillMap, const map2D_vec3 &hillsNormalMap);
  void initializeModelRenderChunks(const map2D_f &landMap, const map2D_f &hillMap);
  void prepareIndirectBufferData(const glm::vec2& cameraPositionXZ, const Frustum& viewFrustum);
  void updateIndirectBufferData();
  void draw(const glm::vec3 &lightDir,
            const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
            const glm::mat4 &projectionView,
            const glm::vec3 &viewPosition,
            bool usePhongShading,
            bool useShadows,
            bool useLandBlending);
  void drawDepthmap(bool grassCastShadow);

  //save/load routine
  void serialize(std::ofstream& output);
  void deserialize(std::ifstream& input);

private:
  //define possible state for plants
  enum PLANT_TYPE : int
  {
    PLANT_STATIC = 0,
    PLANT_ANIMATED = 1
  };
  void prepareDistributionMap();

  map2D_i distributionMap;
  PlantsShader shaders;
  LandPlantsGenerator landPlantsGenerator;
  GrassGenerator grassGenerator;
  HillTreesGenerator hillTreesGenerator;
  TreesRenderer treesRenderer;
  GrassRenderer grassRenderer;
};
