#ifndef THESUNFACADE_H
#define THESUNFACADE_H
#include "game/world/ambience/theSun/TheSun.h"
#include "game/world/ambience/theSun/TheSunShader.h"
#include "game/world/ambience/theSun/TheSunRenderer.h"
#include "graphics/ScreenResolution.h"

constexpr float DEFAULT_SUN_POINT_SIZE = 64.0f;

class TheSunFacade
{
public:
  TheSunFacade(Shader& renderShader, const ScreenResolution& screenResolution);
  void move(float angleDegrees);
  void moveAbsolutePosition(float angleDegrees);
  void draw(const glm::mat4 &skyProjectionView, bool doOcclusionTest, bool useReflectionPointSize);
  const glm::vec3 &getPosition() const;
  const glm::mat4 &getRotationTransform() const;
  GLfloat getSunVisibility(bool multisampled) const;

private:
  TheSun theSun;
  TheSunShader shader;
  TheSunRenderer renderer;
  GLfloat maxSamplesPassedMultisampling, maxSamplesPassed;
};

#endif // THESUNFACADE_H
