#include "TerrainTile.h"

TerrainTile::TerrainTile(int x, int y, float lowLeft, float lowRight, float upperRight, float upperLeft, bool orderCrossed)
  :
    mapX(x),
    mapY(y),
    lowLeft(lowLeft),
    lowRight(lowRight),
    upperRight(upperRight),
    upperLeft(upperLeft),
    orderCrossed(orderCrossed)
{

}

bool operator<(const TerrainTile& a, const TerrainTile& b)
{
  if (a.mapY < b.mapY)
    return true;
  else if (a.mapY == b.mapY)
    return a.mapX < b.mapX;
  else
    return false;
}
