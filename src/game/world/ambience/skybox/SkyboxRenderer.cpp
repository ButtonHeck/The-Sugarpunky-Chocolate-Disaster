#include "SkyboxRenderer.h"

SkyboxRenderer::SkyboxRenderer(Skybox &skybox)
  :
    skybox(skybox)
{}

void SkyboxRenderer::render()
{
  BENCHMARK("SkyboxRenderer: draw", true);
  RendererStateManager::setAmbienceRenderingState(true);
  skybox.basicGLBuffers.bind(VAO);
  glDrawArrays(GL_TRIANGLES, 0, VERTICES_PER_TILE * VERTICES_PER_TILE);
  RendererStateManager::setAmbienceRenderingState(false);
}
