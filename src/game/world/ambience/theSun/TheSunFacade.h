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
  void moveAbsolutePosition(float posDegrees);
  void draw(glm::mat4& skyProjectionView, bool doOcclusionTest, bool useReflectionPointSize);
  glm::vec3 getCurrentPosition() const;
  const glm::mat4 getRotationTransform() const;
  GLfloat getSunVisibilityPercentage();
  void adjustSunPointSize(float pointSizeDivisor, float relfectionPointSizeDivisor);
private:
  TheSunShader shader;
  TheSun theSun;
  TheSunRenderer renderer;
  glm::mat4 rotationTransform;
  GLfloat maxSamplesPassed;
};

#endif // THESUNFACADE_H
