#include "WaterRenderer.h"

WaterRenderer::WaterRenderer(WaterShader &shaders, WaterGenerator &generator)
  :
    shaders(shaders),
    generator(generator)
{}

void WaterRenderer::render(bool useCulling)
{
  if (useCulling)
    {
      {
        shaders.cullingShader.use();
        generator.basicGLBuffers.bind(VAO);
        {
          BENCHMARK("WaterRenderer: draw to TFB", true);
          glEnable(GL_RASTERIZER_DISCARD);
          glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, generator.culledBuffers.get(TFBO));
          glBeginTransformFeedback(GL_TRIANGLES);
          glDrawElements(GL_TRIANGLES, generator.tiles.size() * VERTICES_PER_TILE, GL_UNSIGNED_INT, 0);
          glEndTransformFeedback();
          glDisable(GL_RASTERIZER_DISCARD);
        }
      }
      {
        BENCHMARK("WaterRenderer: draw from TFB", true);
        shaders.renderShader.use();
        generator.culledBuffers.bind(VAO);
        glEnable(GL_BLEND);
        glDrawTransformFeedback(GL_TRIANGLES, generator.culledBuffers.get(TFBO));
        glDisable(GL_BLEND);
      }
    }
  else
    {
      BENCHMARK("WaterRenderer: draw", true);
      shaders.renderShader.use();
      generator.basicGLBuffers.bind(VAO);
      glEnable(GL_BLEND);
      glDrawElements(GL_TRIANGLES, generator.tiles.size() * VERTICES_PER_TILE, GL_UNSIGNED_INT, 0);
      glDisable(GL_BLEND);
    }
}

void WaterRenderer::debugRender(GLenum primitiveType)
{
  BENCHMARK("WaterRenderer: draw normals", true);
  generator.basicGLBuffers.bind(VAO);
  glLineWidth(2.0f);
  glDisable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDrawElements(primitiveType, generator.tiles.size() * VERTICES_PER_TILE, GL_UNSIGNED_INT, 0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_CULL_FACE);
  glLineWidth(1.0f);
}
