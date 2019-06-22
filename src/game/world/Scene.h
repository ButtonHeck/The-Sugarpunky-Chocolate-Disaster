/*
 * Copyright 2019 Ilya Malgin
 * Scene.h
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
 * Purpose: contains declaration for Scene class
 * @version 0.1.0
 */

#pragma once

#include "LandFacade"
#include "HillsFacade"
#include "PlantsFacade"
#include "SkysphereFacade"
#include "WaterFacade"
#include "BuildableFacade"
#include "ShoreFacade"
#include "UnderwaterFacade"
#include "SkyboxFacade"
#include "TheSunFacade"
#include "LensFlareFacade"

class ShaderManager;
class TextureManager;
class MouseInputManager;
class Camera;
class ScreenResolution;
class Options;
class ShadowVolume;

/**
* @brief Game scene. Responsible for initializing and managing all the game objects and subsystems, render ordering,
* handling diffrent rendering modes (onscreen, reflection/refraction, depthmap etc.)
*/
class Scene
{
public:
  Scene(ShaderManager& shaderManager, Options& options, TextureManager& textureManager, const ScreenResolution& screenResolution, const ShadowVolume& shadowVolume);

  //subsystems functions
  void setup();
  void recreate();
  void load();
  void serialize(std::ofstream& output);
  void deserialize(std::ifstream& input);

  //rendering stuff
  void drawWorld(const glm::mat4 &projectionView,
                 const glm::mat4 &ambienceProjectionView,
                 const Frustum &viewFrustum,
                 const Frustum &cullingViewFrustum,
                 const Camera &camera,
                 MouseInputManager& mouseInput);
  void drawWorldDepthmap(bool grassCastShadow);
  void drawWorldReflection(const glm::mat4 &projectionView,
                           const glm::mat4 &ambienceProjectionView,
                           const Frustum &cullingViewFrustum,
                           const Camera &camera);
  void drawWorldRefraction(const glm::mat4 &projectionView);

  //getters
  WaterFacade &getWaterFacade() noexcept;
  HillsFacade &getHillsFacade() noexcept;
  PlantsFacade &getPlantsFacade() noexcept;
  TheSunFacade &getSunFacade() noexcept;
  SkysphereFacade &getSkysphereFacade() noexcept;
  LandFacade &getLandFacade() noexcept;

private:
  ShaderManager& shaderManager;
  Options& options;
  TextureManager& textureManager;
  const ShadowVolume& shadowVolume;

  WaterFacade waterFacade;
  HillsFacade hillsFacade;
  ShoreFacade shoreFacade;
  BuildableFacade buildableFacade;
  PlantsFacade plantsFacade;
  SkyboxFacade skyboxFacade;
  TheSunFacade theSunFacade;
  UnderwaterFacade underwaterFacade;
  LandFacade landFacade;
  LensFlareFacade lensFlareFacade;
  SkysphereFacade skysphereFacade;
};
