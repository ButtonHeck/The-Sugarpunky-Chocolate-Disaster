#pragma once

struct TerrainTile
{
  TerrainTile(int x, int y, float lowLeft, float lowRight, float upperRight, float upperLeft) noexcept;
  const int mapX, mapY;
  const float lowLeft, lowRight, upperRight, upperLeft;
};
