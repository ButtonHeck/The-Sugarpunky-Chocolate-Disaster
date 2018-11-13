#ifndef THESUNFACADE_H
#define THESUNFACADE_H
#include "game/world/ambience/theSun/TheSun.h"
#include "game/world/ambience/theSun/TheSunShader.h"
#include "game/world/ambience/theSun/TheSunRenderer.h"

class TheSunFacade
{
public:
  TheSunFacade(Shader& renderShader);
  void move();
  void draw(glm::mat4& skyProjectionView);
  glm::vec3 getCurrentPosition() const;
private:
  TheSunShader shader;
  TheSun theSun;
  TheSunRenderer renderer;
  glm::mat4 model;
};

#endif // THESUNFACADE_H
