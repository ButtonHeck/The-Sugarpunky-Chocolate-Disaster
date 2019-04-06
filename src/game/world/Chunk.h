#ifndef CHUNK_H
#define CHUNK_H
#include <glm/vec2.hpp>
#include "game/world/SceneSettings.h"

class Chunk
{
public:
  Chunk(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom, float height);
  glm::vec2 getMidPoint() const;

protected:
  unsigned int left, right, top, bottom;
  float midPointX, midPointZ, height;
};

inline glm::vec2 Chunk::getMidPoint() const
{
  return glm::vec2(midPointX, midPointZ);
}

#endif // CHUNK_H
