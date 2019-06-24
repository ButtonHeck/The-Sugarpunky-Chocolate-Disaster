/*
 * Copyright 2019 Ilya Malgin
 * UnderwaterFacade.h
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
 * Purpose: contains declaration for UnderwaterFacade class
 * @version 0.1.0
 */

#pragma once

#include "UnderwaterGenerator"
#include "UnderwaterShader"
#include "UnderwaterRenderer"

/**
* @brief facade for underwater, the simplest one among other terrain facades.
* Responsible for delegating underwater related routines to member submodules
*/
class UnderwaterFacade
{
public:
  UnderwaterFacade(Shader& renderShader) noexcept;
  void draw(const glm::vec3 &lightDir, const glm::mat4 &projectionView);

private:
  UnderwaterShader shader;
  UnderwaterGenerator generator;
  UnderwaterRenderer renderer;
};
