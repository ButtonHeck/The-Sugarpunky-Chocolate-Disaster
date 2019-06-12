#include "TerrainTile"

TerrainTile::TerrainTile(int x, int y, float lowLeft, float lowRight, float upperRight, float upperLeft) noexcept
  :
    mapX(x),
    mapY(y),
    lowLeft(lowLeft),
    lowRight(lowRight),
    upperRight(upperRight),
    upperLeft(upperLeft)
{}
