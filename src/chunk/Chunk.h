#ifndef CHUNK_H
#define CHUNK_H
#include <glm/vec2.hpp>
#include "src/game/Settings.h"

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

inline bool Chunk::containsPoint(float x, float y) const
{
  if (y >= top && y < bottom)
    return x >= left && x < right;
  else
    return false;
}

inline unsigned int Chunk::getLeft() const
{
  return left;
}

inline unsigned int Chunk::getRight() const
{
  return right;
}

inline unsigned int Chunk::getTop() const
{
  return top;
}

inline unsigned int Chunk::getBottom() const
{
  return bottom;
}

inline glm::vec2 Chunk::getMidPoint() const
{
  return glm::vec2(midPointX, midPointY);
}

#endif // CHUNK_H
