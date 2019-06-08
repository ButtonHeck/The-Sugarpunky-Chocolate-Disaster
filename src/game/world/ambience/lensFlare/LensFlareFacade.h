/*
 * Copyright 2019 Ilya Malgin
 * LensFlareFacade.h
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
 * Purpose: contains declaration for LensFlareFacade class
 * @version 0.1.0
 */

#ifndef LENSFLAREFACADE_H
#define LENSFLAREFACADE_H
#include <vector>
#include "LensFlareElement"
#include "LensFlareRenderer"
#include "LensFlareShader"
#include "BufferCollection"
#include "TextureLoader"
#include "BindlessTextureManager"

constexpr unsigned int NUM_LENS_FLARES = 7;

/**
 * @brief Represents a facade for the lens flare effect related code, contains a buffer of flares data
 * and dedicated shader/renderer objects. Responsible for delegating semantically different tasks between
 * shader and renderer objects and updating it's own vertex buffer
 */
class LensFlareFacade
{
public:
  LensFlareFacade(Shader &shader, TextureLoader& textureLoader, const ScreenResolution& screenResolution);
  void draw(const glm::vec3 &sunWorldPosition, const glm::mat4 &projectionView, float brightnessAbs);

private:
  const float BRIGHTNESS_HALO, BRIGHTNESS_FLARES, FLARES_SPACING;
  void updateFlaresPositions(const glm::vec2 &sunScreenPosition, const glm::vec2 &sunToCenter);
  std::vector<LensFlareElement> flares;
  GLfloat vertices[NUM_LENS_FLARES * 3];
  BufferCollection basicGLBuffers;
  LensFlareShader shader;
  LensFlareRenderer renderer;
};

#endif // LENSFLAREFACADE_H
