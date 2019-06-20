/*
 * Copyright 2019 Ilya Malgin
 * HillsFacade.cpp
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
 * Purpose: contains definitions for HillsFacade class
 * @version 0.1.0
 */

#include "HillsFacade"

/**
* @brief plain ctor. Constructs all the member subsystems.
* @param renderShader shader program used during rendering (no frustum culling)
* @param cullingShader shader program used during offscreen rendering with frustum culling
* @param normalsShader shader program used during normals visualization rendering
* @param waterMap map of the water tiles
*/
HillsFacade::HillsFacade(Shader &renderShader, Shader &cullingShader, Shader &normalsShader, const map2D_f &waterMap)
  :
    shaders(renderShader, cullingShader, normalsShader),
    generator(shaders, waterMap),
    renderer(shaders, generator)
{}

/**
* @brief sends setup command to generator
*/
void HillsFacade::setup()
{
  generator.setup();
}

/**
* @brief sends recreation command to generator
*/
void HillsFacade::recreateTilesAndBufferData()
{
  generator.updateMaxHeight();
  generator.createTiles();
  generator.createAuxiliaryMaps();
  generator.fillBufferData();
}

/**
* @brief delegates serialization call to generator
* @param output file stream to write data to
*/
void HillsFacade::serialize(std::ofstream &output)
{
  generator.serialize(output, true, 4);
}

/**
* @brief delegates deserialization call to generator
* @param input file stream to read data from
*/
void HillsFacade::deserialize(std::ifstream &input)
{
  generator.deserialize(input);
}

/**
* @brief handles hill drawing routine: prepares shader and delegates a draw command to renderer
* @param lightDir sunlight direction vector
* @param lightSpaceMatrices array of 3 "projection * view" matrices from the Sun point of view
* @param projectionView "projection * view" matrix
* @param viewPosition position vector of the camera
* @param viewAcceleration a pair of camera's acceleration values (on X and Y axis), used for frustum culling
* @param viewFrustum view frustum of the camera
* @param useFrustumCulling indicator of whether to use frustum culling when drawing
* @param useShadows indicator of whether shadows should be calculated
* @param useDebugRender indicator of whether to draw additional details (mesh grid and normals), used for visual debug only
* @todo remove debug render mode in the release version of the game
*/
void HillsFacade::draw(const glm::vec3 &lightDir,
                       const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
                       const glm::mat4& projectionView,
                       const glm::vec3 &viewPosition,
                       const glm::vec2 &viewAcceleration,
                       const Frustum &viewFrustum,
                       bool useFrustumCulling,
                       bool useShadows,
                       bool useDebugRender)
{
  shaders.update(lightDir,
                 lightSpaceMatrices,
                 projectionView,
                 viewPosition,
                 viewFrustum,
                 generator.maxHeight,
                 useFrustumCulling,
                 useShadows);
  shaders.debugRenderMode(false);
  renderer.render(useFrustumCulling, viewAcceleration);

  if (useDebugRender)
    {
      shaders.debugRenderMode(true);
      renderer.debugRender(GL_TRIANGLES);
      shaders.updateNormals(projectionView);
      renderer.debugRender(GL_POINTS);
    }
}

/**
* @brief delegates draw depthmap command to renderer
*/
void HillsFacade::drawDepthmap()
{
  renderer.renderDepthmap();
}

const map2D_f &HillsFacade::getMap() const noexcept
{
  return generator.getMap();
}

const map2D_vec3 &HillsFacade::getNormalMap() const noexcept
{
  return generator.normalMap;
}
