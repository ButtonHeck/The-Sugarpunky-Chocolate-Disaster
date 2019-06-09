#pragma once

#include <glm/vec2.hpp>

class Frustum;

class Chunk
{
public:
  Chunk(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom, float height);
  glm::vec2 getMidPoint() const;
  unsigned int getLeft() const;
  unsigned int getRight() const;
  unsigned int getTop() const;
  unsigned int getBottom() const;
  void setHeight(float height);
  bool isInsideFrustum(const Frustum &frustum, float cullingOffset) const;

protected:
  unsigned int left, right, top, bottom;
  float midPointX, midPointZ, height;
};

inline glm::vec2 Chunk::getMidPoint() const
{
  return glm::vec2(midPointX, midPointZ);
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

inline void Chunk::setHeight(float height)
{
  this->height = height;
}
