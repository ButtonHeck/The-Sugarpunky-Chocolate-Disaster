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
  void deleteGLObjects() override;
  std::vector<TerrainTile>& getChunkTiles(int i);
  GLuint& getInstanceVAO(int i);
  int getNumInstances(int i);
private:
  GLuint instanceVao[5], instanceVbo[5], instanceEbo[5], instanceModelVbo[5];
  std::vector<std::vector<float>>& waterMap;
  std::vector<std::vector<float>>& hillMap;
  std::vector<TerrainTile> baseChunkTiles[5];
  std::vector<std::vector<float>> chunkMap;
  int NUM_INSTANCES[5];
  void generateMap();
  void smoothMap();
  void correctMapAtEdges();
  void compressMap(float ratio, bool entireRange);
  void denyMapInvisibleTiles();
  void splitMapToChunks(std::vector<TerrainTile>& baseChunks, int chunkSize, bool overlap);
  void removeUnderwaterTiles(float thresholdValue);
};

#endif // BASEMAPGENERATOR_H
