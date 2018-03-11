#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H
#include <vector>
#include <GL/glew.h>
#include "TerrainTile.h"
#include "Settings.h"
#include "Shader.h"

class MapGenerator
{
public:
  MapGenerator();
  void initializeMap();
  void resetAllGLBuffers();
  void setupGLBuffersAttributes();
  void createTiles(bool flat, bool createOnZeroTiles);
  std::vector<std::vector<float>>& getMap();
  std::vector<TerrainTile>& getTiles();
protected:
  std::vector<std::vector<float>> map;
  std::vector<TerrainTile> tiles;
};

#endif // MAPGENERATOR_H
