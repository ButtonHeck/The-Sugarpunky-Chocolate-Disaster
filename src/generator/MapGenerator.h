#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H
#include <vector>
#include <fstream>
#include <GL/glew.h>
#include "model/TerrainTile.h"
#include "game/Settings.h"

class MapGenerator
{
public:
  MapGenerator();
  virtual ~MapGenerator();
  void initializeMap(std::vector<std::vector<float>>& map);
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

#endif // MAPGENERATOR_H
