#include "game/world/ambience/skysphere/Skysphere.h"

Skysphere::Skysphere(const std::string &path, const glm::mat4 &initialTransform)
  :
    model(path, false),
    modelRotationTransform(initialTransform)
{}

void Skysphere::rotate(float angleDegrees, const glm::vec3 &rotationVector)
{
  modelRotationTransform = glm::rotate(modelRotationTransform, glm::radians(angleDegrees), rotationVector);
}

void Skysphere::draw()
{
  model.drawOneInstance();
}

const glm::mat4 &Skysphere::getRotationTransform() const
{
  return modelRotationTransform;
}
