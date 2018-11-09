#include "HillsRenderer.h"

HillsRenderer::HillsRenderer(HillsShader &shaders, HillsGenerator &generator)
  :
    shaders(shaders),
    generator(generator)
{}

void HillsRenderer::render(bool useFC)
{
  if (useFC)
    {
      GLuint tfb = generator.culledBuffers.get(TFBO);
      {
        BENCHMARK("HillsRenderer: draw to TFB", true);
        shaders.cullingShader.use();
        generator.basicGLBuffers.bind(VAO);
        glEnable(GL_RASTERIZER_DISCARD);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, tfb);
        glBeginTransformFeedback(GL_TRIANGLES);
        glDrawArrays(GL_TRIANGLES, 0, generator.tiles.size() * VERTICES_PER_TILE);
        glEndTransformFeedback();
        glDisable(GL_RASTERIZER_DISCARD);
      }
      {
        BENCHMARK("HillsRenderer: draw from TFB", true);
        shaders.renderShader.use();
        generator.culledBuffers.bind(VAO);
        glDrawTransformFeedback(GL_TRIANGLES, tfb);
      }
    }
  else
    {
      BENCHMARK("HillsRenderer: draw", true);
      shaders.renderShader.use();
      generator.basicGLBuffers.bind(VAO);
      glDrawArrays(GL_TRIANGLES, 0, generator.tiles.size() * VERTICES_PER_TILE);
    }
}

void HillsRenderer::renderDepthmap()
{
  BENCHMARK("HillsRenderer: render depthmap", true);
  generator.basicGLBuffers.bind(VAO);
  glDrawArrays(GL_TRIANGLES, 0, generator.tiles.size() * VERTICES_PER_TILE);
}

void HillsRenderer::debugRender(GLenum primitiveType)
{
  generator.basicGLBuffers.bind(VAO);
  glLineWidth(2.0f);
  glDisable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDrawArrays(primitiveType, 0, generator.tiles.size() * VERTICES_PER_TILE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_CULL_FACE);
  glLineWidth(1.0f);
}
