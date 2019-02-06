#include "game/world/terrain/hills/HillsRenderer.h"

HillsRenderer::HillsRenderer(HillsShader &shaders, HillsGenerator &generator)
  :
    shaders(shaders),
    generator(generator)
{}

void HillsRenderer::render(bool useFrustumCulling, const glm::vec2& viewAcceleration)
{
  if (useFrustumCulling)
    {
      float accelerationAbs = glm::max(glm::abs(viewAcceleration.x), glm::abs(viewAcceleration.y));
      int updateOffset = 1;
      if (accelerationAbs > 0.5f && accelerationAbs <= 5.0f)
        updateOffset = 4;
      else if (accelerationAbs > 5.0f && accelerationAbs <= 10.0f)
        updateOffset = 5;
      else if (accelerationAbs > 10.0f)
        updateOffset = 6;
      static unsigned int updateTransformFeedbackCounter = 0;
      GLuint transformFeedback = generator.culledBuffers.get(TFBO);
      if (updateTransformFeedbackCounter == 0)
        {
          BENCHMARK("HillsRenderer: draw to TFB", true);
          shaders.cullingShader.use();
          generator.basicGLBuffers.bind(VAO);
          glEnable(GL_RASTERIZER_DISCARD);
          glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transformFeedback);
          glBeginTransformFeedback(GL_TRIANGLES);
          glDrawElements(GL_TRIANGLES, generator.tiles.size() * VERTICES_PER_QUAD, GL_UNSIGNED_INT, 0);
          glEndTransformFeedback();
          glDisable(GL_RASTERIZER_DISCARD);
        }
      updateTransformFeedbackCounter += updateOffset;
      if (updateTransformFeedbackCounter >= FEEDBACK_UPDATE_DELAY)
        updateTransformFeedbackCounter = 0;
      {
        BENCHMARK("HillsRenderer: draw from TFB", true);
        shaders.renderShader.use();
        generator.culledBuffers.bind(VAO);
        glDrawTransformFeedback(GL_TRIANGLES, transformFeedback);
      }
    }
  else
    {
      BENCHMARK("HillsRenderer: draw", true);
      shaders.renderShader.use();
      generator.basicGLBuffers.bind(VAO);
      glDrawElements(GL_TRIANGLES, generator.tiles.size() * VERTICES_PER_QUAD, GL_UNSIGNED_INT, 0);
    }
}

void HillsRenderer::renderDepthmap()
{
  BENCHMARK("HillsRenderer: render depthmap", true);
  generator.basicGLBuffers.bind(VAO);
  glDrawElements(GL_TRIANGLES, generator.tiles.size() * VERTICES_PER_QUAD, GL_UNSIGNED_INT, 0);
}

void HillsRenderer::debugRender(GLenum primitiveType)
{
  generator.basicGLBuffers.bind(VAO);
  glLineWidth(2.0f);
  glDisable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDrawElements(primitiveType, generator.tiles.size() * VERTICES_PER_QUAD, GL_UNSIGNED_INT, 0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_CULL_FACE);
  glLineWidth(1.0f);
}
