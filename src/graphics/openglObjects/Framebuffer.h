/*
 * Copyright 2019 Ilya Malgin
 * Framebuffer.h
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
 * Purpose: contains declaration for Framebuffer class
 * @version 0.1.0
 */

#pragma once

#include <GL/glew.h>

class TextureManager;

/**
* @brief basic framebuffer. Responsible for holding and validating its FBO and binding/unbinding to a viewport.
* Any bindings and other initialization should be done in descendent framebuffer classes
*/
class Framebuffer
{
public:
  Framebuffer(TextureManager& textureManager) noexcept;
  virtual ~Framebuffer();
  virtual void setup() = 0;
  void checkStatus();
  void bindToViewport(int viewportWidth, int viewportHeight) noexcept;
  void unbindToViewport(int viewportWidth, int viewportHeight) noexcept;

protected:
  GLuint fbo;
  TextureManager& textureManager;
};
