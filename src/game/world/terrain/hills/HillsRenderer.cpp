#include "HillsRenderer.h"

HillsRenderer::HillsRenderer(HillsShader &shaders, std::shared_ptr<HillsGenerator> generator)
  :
    shaders(shaders),
    generator(generator)
{}

void HillsRenderer::render(bool useFC)
{
  if (useFC)
    {
      GLuint tfb = generator->culledBuffers.get(TFBO);
      {
        BENCHMARK("Renderer: draw hills to TFB", true);
        shaders.cullingShader.use();
        glBindVertexArray(generator->basicGLBuffers.get(VAO));
        glEnable(GL_RASTERIZER_DISCARD);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, tfb);
        glBeginTransformFeedback(GL_TRIANGLES);
        glDrawArrays(GL_TRIANGLES, 0, generator->tiles.size() * VERTICES_PER_TILE);
        glEndTransformFeedback();
        glDisable(GL_RASTERIZER_DISCARD);
      }
      {
        BENCHMARK("Renderer: draw hills from TFB", true);
        shaders.renderShader.use();
        glBindVertexArray(generator->culledBuffers.get(VAO));
        glDrawTransformFeedback(GL_TRIANGLES, tfb);
      }
    }
  else
    {
      shaders.renderShader.use();
      glBindVertexArray(generator->basicGLBuffers.get(VAO));
      glDrawArrays(GL_TRIANGLES, 0, generator->tiles.size() * VERTICES_PER_TILE);
    }
}

void HillsRenderer::renderDepthmap()
{
  glBindVertexArray(generator->basicGLBuffers.get(VAO));
  glDrawArrays(GL_TRIANGLES, 0, generator->tiles.size() * VERTICES_PER_TILE);
}
