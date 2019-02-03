#ifndef SKYBOXFACADE_H
#define SKYBOXFACADE_H
#include "game/world/ambience/skybox/Skybox.h"
#include "game/world/ambience/skybox/SkyboxShader.h"
#include "util/BenchmarkTimer.h"

class SkyboxFacade
{
public:
  SkyboxFacade(Shader& renderShader);
  void draw(const glm::mat4 &skyboxProjectionView,
            const glm::vec3 &viewPosition,
            const glm::vec3 &lightDir);

private:
  enum SKYBOX_TYPE : int
  {
    SKYBOX_HILLS_NEAR = 0,
    SKYBOX_HIILS_FAR = 1,
    SKYBOX_CLOUDS = 2
  };
  SkyboxShader shader;
  Skybox skybox;
};

#endif // SKYBOXFACADE_H
