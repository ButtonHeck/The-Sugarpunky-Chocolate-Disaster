#include "game/world/terrain/water/WaterRenderer.h"

WaterRenderer::WaterRenderer(WaterShader &shaders, WaterGenerator &generator)
  :
    shaders(shaders),
    generator(generator),
    anySamplesPassedQuery(GL_ANY_SAMPLES_PASSED)
{}

void WaterRenderer::render(bool useFrustumCulling)
{
  if (useFrustumCulling)
    {
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
      BENCHMARK("WaterRenderer: draw", true);
      shaders.renderShader.use();
      generator.basicGLBuffers.bind(VAO);
      glEnable(GL_BLEND);
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

bool WaterRenderer::anySamplesPassed() const
{
  return anySamplesPassedQuery.getResult() == GL_TRUE;
}
