#ifndef BASEMAPGENERATOR_H
#define BASEMAPGENERATOR_H
#include "MapGenerator.h"
#include <random>

class BaseMapGenerator : public MapGenerator
{
public:
  BaseMapGenerator(std::vector<std::vector<float>>& waterMap, std::vector<std::vector<float>>& hillMap);
  void prepareMap();
  void fillBufferData();
  void fillChunkBufferData();
  void fillCellBufferData();
  void deleteGLObjects() override;
  std::vector<TerrainTile>& getChunkTiles(int i);
  GLuint& getChunkVAO(int i);
  GLuint& getCellVAO();
  int getNumChunksInstances(int i);
  int getNumCellInstances();
private:
  GLuint instanceVao[5], instanceVbo[5], instanceEbo[5], instanceModelVbo[5];
  GLuint cellVao, cellVbo, cellEbo, cellModelVbo;
  std::vector<std::vector<float>>& waterMap;
  std::vector<std::vector<float>>& hillMap;
  std::vector<TerrainTile> baseChunkTiles[5];
  std::vector<std::vector<float>> chunkMap;
  std::vector<TerrainTile> cellTiles;
  std::default_random_engine randomizer;
  int NUM_CHUNKS_INSTANCES[5];
  int NUM_CELL_INSTANCES;
  void generateMap();
  void smoothMap();
  void correctMapAtEdges();
  void compressMap(float ratio, bool entireRange);
  void splitMapToChunks(std::vector<TerrainTile>& baseChunks, int chunkSize, bool overlap);
  void removeUnderwaterTiles(float thresholdValue);
  void split1x1Tiles();
};

#endif // BASEMAPGENERATOR_H
