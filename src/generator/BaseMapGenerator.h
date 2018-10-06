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
  BaseMapGenerator(std::vector<std::vector<float>>& waterMap, std::vector<std::vector<float>>& hillMap);
  ~BaseMapGenerator();
  void setup();
  std::vector<TerrainChunk>& getShoreChunks();
  std::vector<TerrainChunk>& getSquareChunks();
  std::vector<TerrainChunk>& getCellChunks();
  std::vector<TerrainTile>& getSquareTiles();
  std::vector<TerrainTile>& getCellTiles();
  GLuint& getShoreVao();
  size_t getShoreVerticesToDraw() const;
  GLuint& getSquareVAO();
  GLuint& getCellVAO();
  GLuint& getCellDIBO();
  std::vector<std::vector<float>>& getChunkMap();
  int getNumSquareInstances();
  int getNumCellInstances();
  virtual void deserialize(std::ifstream& input) override;
private:
  GLuint squareVao, squareVbo, squareEbo, squareModelVbo;
  GLuint cellVao, cellVbo, cellEbo, cellModelVbo, cellMultiDE_I_DIBO;
  GLuint shoreVao, shoreVbo;
  size_t shoreVerticesToDraw = 0;
  std::vector<std::vector<float>>& waterMap;
  std::vector<std::vector<float>>& hillMap;
  std::vector<TerrainTile> squareTiles;
  std::vector<std::vector<float>> chunkMap;
  std::vector<TerrainTile> cellTiles;
  std::default_random_engine randomizer;
  std::vector<TerrainChunk> squareChunks;
  std::vector<TerrainChunk> cellChunks;
  std::vector<TerrainChunk> shoreChunks;
  std::vector<std::vector<glm::vec3>> shoreNormals;
  int NUM_SQUARE_INSTANCES;
  int NUM_CELL_INSTANCES;
  void generateMap();
  void smoothMap();
  void smoothMapHeightChunks(float baseWeight, float evenWeight, float diagonalWeight);
  void randomizeShore();
  void correctMapAtEdges();
  void compressMap(float ratio);
  void removeUnderwaterTiles(float thresholdValue);
  void splitCellChunks(int chunkSize);
  void splitSquareChunks(int chunkSize);
  void splitShoreChunks(int chunkSize);
  void fillShoreBufferData();
  void fillSquareBufferData();
  void fillCellBufferData();
};

#endif // BASEMAPGENERATOR_H
