#ifndef LENSFLAREELEMENT_H
#define LENSFLAREELEMENT_H
#include <glm/vec2.hpp>
#include <GL/glew.h>

class LensFlareElement
{
public:
  LensFlareElement(float pointSize, GLuint textureID);
  void setPosition(const glm::vec2 &position);
  const glm::vec2& getPosition() const;
  float getPointSize() const;
  GLuint getTextureID() const;
  void adjustPointSize(float pointSizeDivisor);

private:
  glm::vec2 position;
  float pointSize;
  GLuint textureID;
};

#endif // LENSFLAREELEMENT_H
