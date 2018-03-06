#ifndef TERRAIN_TILE_H
#define TERRAIN_TILE_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct TerrainTile
{
  TerrainTile(int x, int y, float lowLeft, float lowRight, float upperRight, float upperLeft, bool orderCrossed = false);
  friend bool operator<(const TerrainTile& a, const TerrainTile& b);
  const int mapX, mapY;
  const float lowLeft, lowRight, upperRight, upperLeft;
  const bool orderCrossed;
};

#endif // TERRAIN_TILE_H
