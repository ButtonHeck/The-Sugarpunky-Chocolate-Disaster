#include "game/world/ambience/lensFlare/LensFlareElement.h"

LensFlareElement::LensFlareElement(float pointSize, GLuint textureID)
  :
    pointSize(pointSize),
    textureID(textureID)
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

GLuint LensFlareElement::getTextureID() const
{
  return textureID;
}

void LensFlareElement::adjustPointSize(float pointSizeDivisor)
{
  pointSize *= pointSizeDivisor;
}
