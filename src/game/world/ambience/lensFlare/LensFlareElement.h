#ifndef LENSFLAREELEMENT_H
#define LENSFLAREELEMENT_H
#include <glm/vec2.hpp>
#include <GL/glew.h>

class LensFlareElement
{
public:
  LensFlareElement(float pointSize);
  void setPosition(const glm::vec2 &position);
  const glm::vec2& getPosition() const;
  float getPointSize() const;
private:
  glm::vec2 position;
  float pointSize;
};

#endif // LENSFLAREELEMENT_H
