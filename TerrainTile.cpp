#include "TerrainTile.h"

TerrainTile::TerrainTile(glm::mat4& model, int x, int y, float lowLeft, float lowRight, float upperRight, float upperLeft, float width, float height, bool orderCrossed)
  :
    model(model),
    mapX(x),
    mapY(y),
    lowLeft(lowLeft),
    lowRight(lowRight),
    upperRight(upperRight),
    upperLeft(upperLeft),
    width(width),
    depth(height),
    orderCrossed(orderCrossed)
{

}
