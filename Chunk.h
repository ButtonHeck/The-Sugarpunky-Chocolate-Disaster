#ifndef CHUNK_H
#define CHUNK_H
#include <glm/glm.hpp>
#include "Settings.h"

class Chunk
{
public:
  Chunk(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom);
  bool containsPoint(float x, float y) const;
  unsigned int getLeft() const;
  unsigned int getRight() const;
  unsigned int getTop() const;
  unsigned int getBottom() const;
  glm::vec2 getMidPoint() const;
protected:
  unsigned int left, right, top, bottom;
  float midPointX, midPointY;
};

#endif // CHUNK_H
