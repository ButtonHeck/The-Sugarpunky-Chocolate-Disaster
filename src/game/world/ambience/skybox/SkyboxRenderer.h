#ifndef SKYBOXRENDERER_H
#define SKYBOXRENDERER_H
#include "game/world/ambience/skybox/Skybox.h"
#include "graphics/Renderer.h"

class SkyboxRenderer
{
public:
  SkyboxRenderer(Skybox& skybox);
  void render();
private:
  Skybox& skybox;
};

#endif // SKYBOXRENDERER_H
