#ifndef GENERATOR_H
#define GENERATOR_H
#include <vector>
#include <fstream>
#include <GL/glew.h>
#include "game/world/terrain/TerrainTile.h"
#include "util/Settings.h"
#include "graphics/OpenglBuffer.h"

template <typename T>
void initializeMap(std::vector<std::vector<T>>& map)
{
  map.clear();
  map.reserve(WORLD_HEIGHT + 1);
  for (size_t row = 0; row < WORLD_HEIGHT + 1; row++)
    map.emplace_back(std::vector<T>(WORLD_WIDTH + 1, 0));
}

void smoothMapHeightChunks(std::vector<std::vector<float>>& map, float selfWeight, float evenWeight, float diagonalWeight);
void smoothNormals(std::vector<std::vector<float>>& map, std::vector<std::vector<glm::vec3>>& normalMap);

class Generator
{
public:
  Generator();
  virtual ~Generator() = default;
  void createTiles(bool flat, bool createOnZeroTiles, std::vector<std::vector<float>>& map, float offsetY);
  std::vector<std::vector<float>>& getMap();
  std::vector<TerrainTile>& getTiles();
  GLuint getVAO();
  GLuint getVBO();
  GLuint getEBO();
  virtual void serialize(std::ofstream& output);
  virtual void deserialize(std::ifstream& input);
protected:
  void resetAllGLBuffers();
  std::vector<std::vector<float>> map;
  std::vector<TerrainTile> tiles;
  OpenglBuffer basicGLBuffers;
};

#endif // GENERATOR_H
