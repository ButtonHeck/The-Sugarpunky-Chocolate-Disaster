#ifndef GENERATOR_H
#define GENERATOR_H
#include <vector>
#include <fstream>
#include <GL/glew.h>
#include "game/world/terrain/TerrainTile.h"
#include "util/Settings.h"
#include "util/typeAliases.h"
#include "graphics/OpenglBuffer.h"

class Generator
{
public:
  Generator();
  virtual ~Generator() = default;
  void createTiles(bool flat, bool createOnZeroTiles, map2D_f& map, float offsetY);
  const map2D_f& getMap() const;
  std::vector<TerrainTile>& getTiles();
  virtual void serialize(std::ofstream& output);
  virtual void deserialize(std::ifstream& input);

  template <typename T>
  static void initializeMap(map2D_template<T>& map)
  {
    map.clear();
    map.reserve(WORLD_HEIGHT + 1);
    for (size_t row = 0; row < WORLD_HEIGHT + 1; row++)
      map.emplace_back(std::vector<T>(WORLD_WIDTH + 1, 0));
  }
  static void smoothMapHeightChunks(map2D_f& map, float selfWeight, float evenWeight, float diagonalWeight);
  static void smoothNormals(map2D_f& map, map2D_vec3& normalMap);

protected:
  void resetAllGLBuffers();
  map2D_f map;
  std::vector<TerrainTile> tiles;
  OpenglBuffer basicGLBuffers;
};

#endif // GENERATOR_H
