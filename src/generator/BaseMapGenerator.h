#ifndef BASEMAPGENERATOR_H
#define BASEMAPGENERATOR_H
#include "MapGenerator.h"
#include <random>
#include "src/chunk/BaseTerrainChunk.h"
#include "src/chunk/CellChunk.h"
#include "src/graphics/Camera.h"

class BaseMapGenerator : public MapGenerator
{
public:
  BaseMapGenerator(std::vector<std::vector<float>>& waterMap, std::vector<std::vector<float>>& hillMap);
  void prepareMap(bool randomizeShoreFlag);
  void fillBufferData();
  void fillChunkBufferData();
  void fillCellBufferData();
  void split1x1Tiles(int chunkSize);
  void splitMapToChunks(int chunkSize);
  void splitShoreToChunks(int chunkSize);
  void deleteGLObjects() override;
  std::vector<CellChunk>& getShoreChunks();
  std::vector<BaseTerrainChunk>& getFlatChunks();
  std::vector<CellChunk>& getCellChunks();
  std::vector<TerrainTile>& getChunkTiles();
  std::vector<TerrainTile>& getCellTiles();
  GLuint& getShoreVao(int i);
  GLuint& getInstanceVao();
  GLuint& getChunkVAO();
  GLuint& getCellVAO();
  std::vector<std::vector<float>>& getChunkMap();
  int getNumChunksInstances();
  int getNumCellInstances();
private:
  GLuint instanceVao, instanceVbo, instanceEbo, instanceModelVbo;
  GLuint cellVao, cellVbo, cellEbo, cellModelVbo;
  std::vector<GLuint> shoreVaos, shoreVbos;
  std::vector<std::vector<float>>& waterMap;
  std::vector<std::vector<float>>& hillMap;
  std::vector<TerrainTile> baseChunkTiles;
  std::vector<std::vector<float>> chunkMap;
  std::vector<TerrainTile> cellTiles;
  std::default_random_engine randomizer;
  std::vector<BaseTerrainChunk> baseChunks;
  std::vector<CellChunk> cellChunks;
  std::vector<CellChunk> shoreChunks;
  int NUM_CHUNKS_INSTANCES;
  int NUM_CELL_INSTANCES;
  void generateMap();
  void smoothMap();
  void randomizeShore();
  void correctMapAtEdges();
  void compressMap(float ratio);
  void removeUnderwaterTiles(float thresholdValue);
};

#endif // BASEMAPGENERATOR_H
