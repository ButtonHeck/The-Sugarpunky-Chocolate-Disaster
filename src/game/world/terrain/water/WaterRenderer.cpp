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
          glDrawArrays(GL_TRIANGLES, 0, generator.tiles.size() * VERTICES_PER_TILE);
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
      glDrawArrays(GL_TRIANGLES, 0, generator.tiles.size() * VERTICES_PER_TILE);
      glDisable(GL_BLEND);
    }
}
