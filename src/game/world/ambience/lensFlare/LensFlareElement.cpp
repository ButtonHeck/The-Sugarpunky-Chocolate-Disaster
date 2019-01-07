#include "game/world/ambience/lensFlare/LensFlareElement.h"

LensFlareElement::LensFlareElement(float pointSize)
  :
    pointSize(pointSize)
{}

void LensFlareElement::setPosition(const glm::vec2 &position)
{
  this->position = position;
}

const glm::vec2 &LensFlareElement::getPosition() const
{
  return position;
}

float LensFlareElement::getPointSize() const
{
  return pointSize;
}
