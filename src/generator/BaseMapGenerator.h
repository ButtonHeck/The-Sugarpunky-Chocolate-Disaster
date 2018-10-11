#ifndef BASEMAPGENERATOR_H
#define BASEMAPGENERATOR_H
#include <random>
#include <chrono>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstring>
#include "MapGenerator.h"
#include "chunk/TerrainChunk.h"

class BaseMapGenerator : public MapGenerator
{
public:
  BaseMapGenerator();
  ~BaseMapGenerator();
  void setup(std::vector<std::vector<float>> &shoreMap);
  std::vector<TerrainChunk>& getCellChunks();
  GLuint& getSquareVAO();
  GLuint& getCellVAO();
  GLuint& getCellDIBO();
  int getNumSquareInstances() const;
  virtual void deserialize(std::ifstream& input) override;
private:
  void generateMap(std::vector<std::vector<float>> &shoreMap);
  void splitCellChunks(int chunkSize);
  void splitSquareChunks(int chunkSize);
  void fillSquareBufferData();
  void fillCellBufferData();
  GLuint squareVao, squareVbo, squareEbo, squareModelVbo;
  GLuint cellVao, cellVbo, cellEbo, cellModelVbo, cellMultiDE_I_DIBO;
  std::vector<TerrainTile> squareTiles;
  std::vector<std::vector<float>> chunkMap;
  std::vector<TerrainTile> cellTiles;
  std::default_random_engine randomizer;
  std::vector<TerrainChunk> squareChunks;
  std::vector<TerrainChunk> cellChunks;
  int NUM_SQUARE_INSTANCES;
};

#endif // BASEMAPGENERATOR_H
