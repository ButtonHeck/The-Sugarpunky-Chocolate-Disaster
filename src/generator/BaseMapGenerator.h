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
  void prepareMap(bool randomizeShoreFlag);
  void fillShoreBufferData();
  void fillSquareBufferData();
  void fillCellBufferData();
  void splitCellChunks(int chunkSize);
  void splitSquareChunks(int chunkSize);
  void splitShoreChunks(int chunkSize);
  void deleteGLObjects() override;
  std::vector<TerrainChunk>& getShoreChunks();
  std::vector<TerrainChunk>& getSquareChunks();
  std::vector<TerrainChunk>& getCellChunks();
  std::vector<TerrainTile>& getSquareTiles();
  std::vector<TerrainTile>& getCellTiles();
  GLuint& getShoreVao();
  size_t getShoreVerticesToDraw() const;
  GLuint& getSquareVAO();
  GLuint& getCellVAO();
  std::vector<std::vector<float>>& getChunkMap();
  int getNumSquareInstances();
  int getNumCellInstances();
private:
  GLuint squareVao, squareVbo, squareEbo, squareModelVbo;
  GLuint cellVao, cellVbo, cellEbo, cellModelVbo;
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
  int NUM_SQUARE_INSTANCES;
  int NUM_CELL_INSTANCES;
  void generateMap();
  void smoothMap();
  void randomizeShore();
  void correctMapAtEdges();
  void compressMap(float ratio);
  void removeUnderwaterTiles(float thresholdValue);
};

#endif // BASEMAPGENERATOR_H
