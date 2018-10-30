#ifndef THESUNFACADE_H
#define THESUNFACADE_H
#include "game/world/ambience/theSun/TheSun.h"
#include "game/world/ambience/theSun/TheSunShader.h"
#include "game/world/ambience/theSun/TheSunRenderer.h"

class TheSunFacade
{
public:
  TheSunFacade(Shader& renderShader);
  void draw(glm::mat4& skyProjectionView);
private:
  TheSunShader shader;
  TheSun theSun;
  TheSunRenderer renderer;
};

#endif // THESUNFACADE_H
