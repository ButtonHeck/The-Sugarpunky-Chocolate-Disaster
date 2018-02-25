#ifndef TERRAIN_TILE_H
#define TERRAIN_TILE_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class TerrainTile
{
public:
  TerrainTile(glm::mat4& model, int x, int y, float lowLeft, float lowRight, float upperRight, float upperLeft, float width, float depth, bool orderCrossed = false);
  const glm::mat4 model;
  const int mapX, mapY;
  const float lowLeft, lowRight, upperRight, upperLeft;
  const float width, depth;
  const bool orderCrossed;
};

#endif // TERRAIN_TILE_H
