/*
 * Copyright 2019 Ilya Malgin
 * BuildableFacade.cpp
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
 * Purpose: contains definition for BuildableFacade class
 * @version 0.1.0
 */

#include "BuildableFacade"
#include "BenchmarkTimer"
#include "MouseInputManager"

/**
* @brief creates a facade for buildable and selected tiles related codebase
* @param buildableRenderShader shader used to render buidable tiles
* @param selectedRenderShader shader used to render selected tile
*/
BuildableFacade::BuildableFacade(Shader &buildableRenderShader, Shader &selectedRenderShader) noexcept
  :
    shader(buildableRenderShader, selectedRenderShader),
    generator(),
    renderer(generator)
{}

/**
* @brief delegates map creation to a generator
* @param landMap map of the land tiles
* @param hillsMap map of the hill tiles
*/
void BuildableFacade::setup(const map2D_f &landMap, const map2D_f &hillsMap)
{
  generator.setup(landMap, hillsMap);
}

/**
* @brief launches buildable tiles rendering routine
* @param projectionView "projection * view" matrix fed to the shader
*/
void BuildableFacade::drawBuildable(const glm::mat4& projectionView)
{
  shader.updateBuildable(projectionView);
  {
    BENCHMARK("Buildable: draw buildable", true);
    renderer.renderBuildable();
  }
}

/**
* @brief launches selected tile rendering routine
* @param projectionView "projection * view" matrix fed to the shader
* @param mouseInput mouse input object used to pick cursor current map coordinates
*/
void BuildableFacade::drawSelected(const glm::mat4& projectionView, MouseInputManager& mouseInput)
{
  if (generator.getMap()[mouseInput.getCursorWorldZ()][mouseInput.getCursorWorldX()] != 0)
    {
      BENCHMARK("Buildable: draw selected", true);
      glm::vec4 translationVector(-HALF_WORLD_WIDTH + mouseInput.getCursorWorldX(), 0.01f, -HALF_WORLD_HEIGHT + mouseInput.getCursorWorldZ(), 0.0f);
      shader.updateSelected(projectionView, translationVector);
      renderer.renderSelected();
    }
}

/**
* @brief getter of the buildable map
*/
const map2D_f &BuildableFacade::getMap() const noexcept
{
  return generator.getMap();
}
