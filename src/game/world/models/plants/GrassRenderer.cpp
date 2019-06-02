/*
 * Copyright 2019 Ilya Malgin
 * GrassRenderer.cpp
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
 * Purpose: contains definitions for GrassRenderer class
 * @version 0.1.0
 */

#include "GrassRenderer.h"

/**
 * @brief just delegates a draw call for each given model and switches GL_CULL_FACE setting
 * @param models vector of models to go through
 * @param isShadow define whether depthmap or plain on-screen rendering mode is on
 */
void GrassRenderer::render(std::vector<Model> &models, bool isShadow)
{
  //grass polygon's back face is the same as front, so make sure no culling is applied here
  glDisable(GL_CULL_FACE);
  for (Model& grassModel : models)
    grassModel.draw(isShadow);
  glEnable(GL_CULL_FACE);
}
