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
  GLuint instanceVao[NUM_BASE_TERRAIN_CHUNKS],
         instanceVbo[NUM_BASE_TERRAIN_CHUNKS],
         instanceEbo[NUM_BASE_TERRAIN_CHUNKS],
         instanceModelVbo[NUM_BASE_TERRAIN_CHUNKS];
  GLuint cellVao, cellVbo, cellEbo, cellModelVbo;
  std::vector<std::vector<float>>& waterMap;
  std::vector<std::vector<float>>& hillMap;
  std::vector<TerrainTile> baseChunkTiles[NUM_BASE_TERRAIN_CHUNKS];
  std::vector<std::vector<float>> chunkMap;
  std::vector<TerrainTile> cellTiles;
  std::default_random_engine randomizer;
  int NUM_CHUNKS_INSTANCES[NUM_BASE_TERRAIN_CHUNKS];
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
