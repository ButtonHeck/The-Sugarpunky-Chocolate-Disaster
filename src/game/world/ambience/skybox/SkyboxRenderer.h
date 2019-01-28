#ifndef SKYBOXRENDERER_H
#define SKYBOXRENDERER_H
#include "game/world/ambience/skybox/Skybox.h"
#include "graphics/RendererStateManager.h"
#include "util/BenchmarkTimer.h"

class SkyboxRenderer
{
public:
  SkyboxRenderer(Skybox& skybox);
  void render();
private:
  constexpr static unsigned int VERTICES_PER_SKYBOX = VERTICES_PER_QUAD * VERTICES_PER_QUAD;
  Skybox& skybox;
};

#endif // SKYBOXRENDERER_H
