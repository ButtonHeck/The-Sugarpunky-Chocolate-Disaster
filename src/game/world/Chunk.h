#pragma once

#include <glm/vec2.hpp>

class Frustum;

class Chunk
{
public:
  Chunk(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom, float height) noexcept;
  glm::vec2 getMidPoint() const;
  unsigned int getLeft() const noexcept;
  unsigned int getRight() const noexcept;
  unsigned int getTop() const noexcept;
  unsigned int getBottom() const noexcept;
  void setHeight(float height) noexcept;
  bool isInsideFrustum(const Frustum &frustum, float cullingOffset) const;

protected:
  unsigned int left, right, top, bottom;
  float midPointX, midPointZ, height;
};

inline glm::vec2 Chunk::getMidPoint() const
{
  return glm::vec2(midPointX, midPointZ);
}

inline unsigned int Chunk::getLeft() const noexcept
{
  return left;
}

inline unsigned int Chunk::getRight() const noexcept
{
  return right;
}

inline unsigned int Chunk::getTop() const noexcept
{
  return top;
}

inline unsigned int Chunk::getBottom() const noexcept
{
  return bottom;
}

inline void Chunk::setHeight(float height) noexcept
{
  this->height = height;
}
