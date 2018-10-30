#ifndef SKYBOXFACADE_H
#define SKYBOXFACADE_H
#include "game/world/ambience/skybox/Skybox.h"
#include "game/world/ambience/skybox/SkyboxShader.h"
#include "game/world/ambience/skybox/SkyboxRenderer.h"

class SkyboxFacade
{
public:
  SkyboxFacade(Shader& renderShader);
  void draw(glm::mat4& skyProjectionView, glm::vec3 &viewPosition);
private:
  SkyboxShader shader;
  Skybox skybox;
  SkyboxRenderer renderer;
};

#endif // SKYBOXFACADE_H
