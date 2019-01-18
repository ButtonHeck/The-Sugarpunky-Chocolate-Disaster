#ifndef THESUNFACADE_H
#define THESUNFACADE_H
#include "game/world/ambience/theSun/TheSun.h"
#include "game/world/ambience/theSun/TheSunShader.h"
#include "game/world/ambience/theSun/TheSunRenderer.h"

constexpr float DEFAULT_SUN_POINT_SIZE = 64.0f;

class TheSunFacade
{
public:
  TheSunFacade(Shader& renderShader);
  void move(float delta);
  void setPos(float posDegrees);
  void draw(glm::mat4& skyProjectionView, bool doOcclusionTest);
  glm::vec3 getCurrentPosition() const;
  GLfloat getSunVisibilityPercentage();
  void adjustSunPointSize(float pointSizeDivisor);
private:
  TheSunShader shader;
  TheSun theSun;
  TheSunRenderer renderer;
  glm::mat4 model;
  GLfloat maxSamplesPassed;
};

#endif // THESUNFACADE_H
