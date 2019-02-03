#ifndef SKYSPHERE_H
#define SKYSPHERE_H
#include "game/world/models/Model.h"

class Skysphere
{
public:
  Skysphere(const std::string& path, const glm::mat4 &initialTransform = glm::mat4());
  void rotate(float angleDegrees, const glm::vec3& rotationVector);
  void draw();
  const glm::mat4& getRotationTransform() const;

private:
  Model model;
  glm::mat4 modelRotationTransform;
};

#endif // SKYSPHERE_H
