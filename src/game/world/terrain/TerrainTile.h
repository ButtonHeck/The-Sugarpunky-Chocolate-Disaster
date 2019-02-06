#ifndef TERRAIN_TILE_H
#define TERRAIN_TILE_H

struct TerrainTile
{
  TerrainTile(int x, int y, float lowLeft, float lowRight, float upperRight, float upperLeft);
  const int mapX, mapY;
  const float lowLeft, lowRight, upperRight, upperLeft;
};

#endif // TERRAIN_TILE_H
