/*
 * Copyright 2019 Ilya Malgin
 * ModelRenderer.cpp
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
 * Purpose: contains definitions for ModelRenderer class
 * @version 0.1.0
 */

#include "ModelRenderer"
#include "BufferCollection"
#include "BenchmarkTimer"

/**
* @brief plain ctor
* @param basicGLBuffers onscreen rendering buffer collection
* @param depthmapDIBO offscreen indirect draw buffer for depthmap rendering
*/
ModelRenderer::ModelRenderer(BufferCollection &basicGLBuffers, BufferCollection &depthmapDIBO) noexcept
  :
    basicGLBuffers(basicGLBuffers),
    depthmapDIBO(depthmapDIBO)
{}

/**
* @brief sends draw calls to OpenGL depending on the current mode
* @param isDepthmap rendering mode
* @param primCount number of instances to render
*/
void ModelRenderer::render(bool isDepthmap, GLsizei primCount)
{
  basicGLBuffers.bind(VAO);
  if (!isDepthmap)
    {
      basicGLBuffers.bind(DIBO);
      BENCHMARK("Model: draw", true);
      glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, primCount, 0);
    }
  else
    {
      depthmapDIBO.bind(DIBO);
      BENCHMARK("Model: draw shadows", true);
      glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, primCount, 0);
    }
}

/**
* @brief sends draw call to OpenGL to render model as the singleton
* @param numIndices number of indices in the model to render
*/
void ModelRenderer::renderOneInstance(GLsizei numIndices)
{
  basicGLBuffers.bind(VAO);
  glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
}
