#ifndef GENERATOR_H
#define GENERATOR_H
#include <vector>
#include <fstream>
#include <GL/glew.h>
#include "model/TerrainTile.h"
#include "util/Settings.h"

void initializeMap(std::vector<std::vector<float>>& map);
void smoothMapHeightChunks(std::vector<std::vector<float>>& map, float selfWeight, float evenWeight, float diagonalWeight);
void smoothNormals(std::vector<std::vector<float>>& map, std::vector<std::vector<glm::vec3>>& normalMap);

class Generator
{
public:
  Generator();
  virtual ~Generator();
  void createTiles(bool flat, bool createOnZeroTiles, std::vector<std::vector<float>>& map, float offsetY);
  std::vector<std::vector<float>>& getMap();
  std::vector<TerrainTile>& getTiles();
  GLuint getVAO() const;
  GLuint getVBO() const;
  GLuint getEBO() const;
  virtual void serialize(std::ofstream& output);
  virtual void deserialize(std::ifstream& input);
protected:
  void resetAllGLBuffers();
  std::vector<std::vector<float>> map;
  std::vector<TerrainTile> tiles;
  GLuint vao, vbo, ebo;
};

#endif // GENERATOR_H
