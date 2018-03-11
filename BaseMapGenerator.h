#ifndef BASEMAPGENERATOR_H
#define BASEMAPGENERATOR_H
#include "MapGenerator.h"
#include <random>

class BaseMapGenerator : public MapGenerator
{
public:
  BaseMapGenerator(std::vector<std::vector<float>>& waterMap, std::vector<std::vector<float>>& hillMap);
  void prepareMap();
  void fillBaseBufferData();
  void fillChunkBufferData();
  void generateBaseTerrainMap();
  void smoothBaseTerrainMap();
  void correctBaseTerrainMapAtEdges();
  void compressHeightBaseTerrainMap(float ratio, bool entireRange);
  void denyBaseTerrainMapInvisibleTiles();
  void splitBaseTerrainToChunks(std::vector<TerrainTile>& baseChunks, int chunkSize, bool overlap);
  void removeBaseTerrainUnderwaterTiles(float thresholdValue);
  void draw(Shader& shader, GLuint& sandTexture);
  void drawChunks(Shader& shader);
  GLuint& getVAO();
  GLuint& getVBO();
  GLuint& getEBO();
  std::vector<TerrainTile>& getChunkTiles(int i);
  void deleteGLObjects();
private:
  GLuint vao, vbo, ebo;
  GLuint instanceVao[5], instanceVbo[5], instanceEbo[5], instanceModelVbo[5];
  std::vector<std::vector<float>>& waterMap;
  std::vector<std::vector<float>>& hillMap;
  std::vector<TerrainTile> baseChunkTiles[5];
  std::vector<std::vector<float>> chunkMap;
  int NUM_INSTANCES[5];
};

#endif // BASEMAPGENERATOR_H
