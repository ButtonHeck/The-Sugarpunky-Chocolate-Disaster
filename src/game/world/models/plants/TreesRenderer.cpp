/*
 * Copyright 2019 Ilya Malgin
 * TreesRenderer.cpp
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
 * Purpose: contains definitions for TreesRenderer class
 * @version 0.1.0
 */

#include "TreesRenderer.h"

/**
 * @brief just delegates a draw call for each given model
 * @param landModels vector of land trees models to go through
 * @param hillModels vector of hills trees models to go through
 * @param isShadow define whether depthmap or plain on-screen rendering mode is on
 */
void TreesRenderer::render(std::vector<Model> &landModels, std::vector<Model> &hillModels, bool isShadow)
{
  for (Model& landModel : landModels)
    landModel.draw(isShadow);
  for (Model& hillModel : hillModels)
    hillModel.draw(isShadow);
}
