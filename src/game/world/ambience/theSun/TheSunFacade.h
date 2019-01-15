#ifndef THESUNFACADE_H
#define THESUNFACADE_H
#include "game/world/ambience/theSun/TheSun.h"
#include "game/world/ambience/theSun/TheSunShader.h"
#include "game/world/ambience/theSun/TheSunRenderer.h"

constexpr float MAX_SUN_SAMPLES_PASSED = SUN_POINT_SIZE * SUN_POINT_SIZE * MULTISAMPLES;

class TheSunFacade
{
public:
  TheSunFacade(Shader& renderShader);
  void move(float delta);
  void setPos(float posDegrees);
  void draw(glm::mat4& skyProjectionView, bool doOcclusionTest);
  glm::vec3 getCurrentPosition() const;
  GLuint getSamplesPassedQueryResult();
private:
  TheSunShader shader;
  TheSun theSun;
  TheSunRenderer renderer;
  glm::mat4 model;
};

#endif // THESUNFACADE_H
