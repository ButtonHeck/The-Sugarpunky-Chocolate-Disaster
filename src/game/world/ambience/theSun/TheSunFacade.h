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
  void move(float angleDegrees);
  void moveAbsolutePosition(float angleDegrees);
  void draw(const glm::mat4 &skyProjectionView, bool doOcclusionTest, bool useReflectionPointSize);
  const glm::vec3 &getPosition() const;
  const glm::mat4 &getRotationTransform() const;
  GLfloat getSunVisibilityPercentage() const;
  void adjustSunPointSize(float pointSizeDivisor, float relfectionPointSizeDivisor);

private:
  TheSun theSun;
  TheSunShader shader;
  TheSunRenderer renderer;
  GLfloat maxSamplesPassed;
};

#endif // THESUNFACADE_H
