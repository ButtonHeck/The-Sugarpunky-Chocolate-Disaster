/*
 * Copyright 2019 Ilya Malgin
 * LensFlareShader.cpp
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
 * Purpose: contains definitions for LensFlareShader class
 * @version 0.1.0
 */

#include "game/world/ambience/lensFlare/LensFlareShader.h"

/**
 * @param renderShader precompiled shader program which is used during rendering
 */
LensFlareShader::LensFlareShader(Shader &renderShader)
  :
    renderShader(renderShader)
{}

/**
 * @brief activates shader program and update its uniforms
 */
void LensFlareShader::update(float brightnessFlares, float brightnessHalo)
{
  renderShader.use();
  renderShader.setFloat("u_brightnessFlare", brightnessFlares);
  renderShader.setFloat("u_brightnessHalo", brightnessHalo);
}
