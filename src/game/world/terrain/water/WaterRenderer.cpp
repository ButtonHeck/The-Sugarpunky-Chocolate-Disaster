/*
 * Copyright 2019 Ilya Malgin
 * WaterRenderer.cpp
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
 * Purpose: contains definitions for WaterRenderer class
 * @version 0.1.0
 */

#include "WaterRenderer"
#include "WaterGenerator"
#include "WaterShader"
#include "Shader"
#include "BenchmarkTimer"

/**
* @brief plain ctor, initializes query member object with GL_ANY_SAMPLES_PASSED target
* @param shaders water shader manager
* @param generator water generator to fetch GL buffer objects from
*/
WaterRenderer::WaterRenderer(WaterShader &shaders, WaterGenerator &generator) noexcept
  :
    shaders(shaders),
    generator(generator),
    anySamplesPassedQuery(GL_ANY_SAMPLES_PASSED)
{}

/**
* @brief handles water rendering process
* @param useFrustumCulling defines whether frustum culling mode of rendering is currently used
*/
void WaterRenderer::render(bool useFrustumCulling)
{
  if (useFrustumCulling)
    {
	  //firstly render plain data offscreen with bound transform feedback object
      shaders.cullingShader.use();
      generator.basicGLBuffers.bind(VAO);
      GLuint transformFeedback = generator.culledBuffers.get(TFBO);
      {
        BENCHMARK("WaterRenderer: draw to TFB", true);
        glEnable(GL_RASTERIZER_DISCARD);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transformFeedback);
        glBeginTransformFeedback(GL_TRIANGLES);
        glDrawElements(GL_TRIANGLES, generator.tiles.size() * VERTICES_PER_QUAD, GL_UNSIGNED_INT, 0);
        glEndTransformFeedback();
        glDisable(GL_RASTERIZER_DISCARD);
      }
      {
        BENCHMARK("WaterRenderer: draw from TFB", true);
        shaders.renderShader.use();
        generator.culledBuffers.bind(VAO);
        glEnable(GL_BLEND);

		//optionally inject query into rendering process if necessary
        if (!anySamplesPassedQuery.isInUse())
          {
            anySamplesPassedQuery.start();
            glDrawTransformFeedback(GL_TRIANGLES, transformFeedback);
            anySamplesPassedQuery.end();
          }
        else
          glDrawTransformFeedback(GL_TRIANGLES, transformFeedback);

        glDisable(GL_BLEND);
      }
    }
  else
    {
	  //just plain rendering without transform feedback
      BENCHMARK("WaterRenderer: draw", true);
      shaders.renderShader.use();
      generator.basicGLBuffers.bind(VAO);
      glEnable(GL_BLEND);

	  //optionally inject query into rendering process if necessary
      if (!anySamplesPassedQuery.isInUse())
        {
          anySamplesPassedQuery.start();
          glDrawElements(GL_TRIANGLES, generator.tiles.size() * VERTICES_PER_QUAD, GL_UNSIGNED_INT, 0);
          anySamplesPassedQuery.end();
        }
      else
        glDrawElements(GL_TRIANGLES, generator.tiles.size() * VERTICES_PER_QUAD, GL_UNSIGNED_INT, 0);

      glDisable(GL_BLEND);
    }

  if (anySamplesPassedQuery.isResultAvailable())
    anySamplesPassedQuery.requestResult();
}

/**
* @brief temporary utility function to render additional visualizations for debugging
* @param primitiveType GL defined primitive type to use for rendering
* @todo remove this from release version of the game
*/
void WaterRenderer::debugRender(GLenum primitiveType)
{
  BENCHMARK("WaterRenderer: draw normals", true);
  generator.basicGLBuffers.bind(VAO);
  glLineWidth(2.0f);
  glDisable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDrawElements(primitiveType, generator.tiles.size() * VERTICES_PER_QUAD, GL_UNSIGNED_INT, 0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_CULL_FACE);
  glLineWidth(1.0f);
}

/**
* @brief helper function that tells whether query object has confirmed that any samples actually passed rendering stage
*/
bool WaterRenderer::anySamplesPassed() const noexcept
{
  return anySamplesPassedQuery.getResult() == GL_TRUE;
}
