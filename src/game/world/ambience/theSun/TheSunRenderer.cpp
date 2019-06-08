/*
 * Copyright 2019 Ilya Malgin
 * TheSunRenderer.cpp
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
 * Purpose: contains definitions for TheSunRenderer class
 * @version 0.1.0
 */

#include "TheSunRenderer"

/**
 * @param theSun entity to peek VAO from during rendering
 */
TheSunRenderer::TheSunRenderer(TheSun& theSun)
  :
    theSun(theSun),
    samplesPassedQuery(GL_SAMPLES_PASSED),
    pointSize(0.0f)
{}

/**
 * @brief perform actual rendering with optional occlusion query
 * @param doOcclusionTest define whether or not occlusion query takes place during rendering
 * @param useReflectionPointSize a flag indicating what point size to use
 */
void TheSunRenderer::render(bool doOcclusionTest, bool useReflectionPointSize)
{
  BENCHMARK("SunRenderer: draw", true);
  glPointSize(useReflectionPointSize ? reflectionPointSize : pointSize);
  theSun.basicGLBuffers.bind(VAO);
  if (doOcclusionTest && !samplesPassedQuery.isInUse())
    {
      samplesPassedQuery.start();
      glDrawArrays(GL_POINTS, 0, 1);
      samplesPassedQuery.end();
    }
  else
    glDrawArrays(GL_POINTS, 0, 1);

  if (samplesPassedQuery.isResultAvailable())
    samplesPassedQuery.requestResult();
}

GLuint TheSunRenderer::getSamplesPassedQueryResult() const
{
  return samplesPassedQuery.getResult();
}

void TheSunRenderer::setPointSize(float pointSize)
{
  this->pointSize = pointSize;
}

void TheSunRenderer::setReflectionPointSize(float pointSize)
{
  this->reflectionPointSize = pointSize;
}

float TheSunRenderer::getPointSize() const
{
  return pointSize;
}
