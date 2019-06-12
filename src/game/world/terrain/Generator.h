#pragma once

#include "TerrainTile"
#include "SceneSettings"
#include "TypeAliases"
#include "BufferCollection"

#include <vector>
#include <fstream>

constexpr unsigned int UNIQUE_VERTICES_PER_TILE = 4;

class Generator
{
public:
  Generator() noexcept;
  virtual ~Generator() = default;
  void createTiles(bool flat, bool createOnZeroTiles, const map2D_f& map, float offsetY);
  const map2D_f& getMap() const noexcept;
  virtual void serialize(std::ofstream& output, bool usePrecision = false, unsigned int precision = 6);
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
  static void smoothNormals(const map2D_f &map, map2D_vec3& normalMap);

protected:
  map2D_f map;
  std::vector<TerrainTile> tiles;
  BufferCollection basicGLBuffers;

private:
  template <typename T>
  void serializeRepeatValues(std::ofstream &output,
                             T value,
                             unsigned int& row,
                             unsigned int& column);
  template <typename T>
  void deserializeRepeatValues(std::ifstream &input,
                               T value,
                               unsigned int& row,
                               unsigned int& column);
};
