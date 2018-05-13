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
  virtual ~MapGenerator() = default;
  void initializeMap(std::vector<std::vector<float>>& map);
  void resetAllGLBuffers();
  void setupGLBuffersAttributes();
  void createTiles(bool flat, bool createOnZeroTiles, std::vector<std::vector<float>>& map, float offsetY = 0.0f);
  std::vector<std::vector<float>>& getMap();
  std::vector<TerrainTile>& getTiles();
  GLuint& getVAO();
  GLuint& getVBO();
  GLuint& getEBO();
  virtual void deleteGLObjects();
protected:
  std::vector<std::vector<float>> map;
  std::vector<TerrainTile> tiles;
  GLuint vao, vbo, ebo;
};

#endif // MAPGENERATOR_H
