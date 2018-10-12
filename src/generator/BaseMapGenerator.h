#ifndef BASEMAPGENERATOR_H
#define BASEMAPGENERATOR_H
#include <random>
#include <chrono>
#include <memory>
#include "MapGenerator.h"
#include "chunk/TerrainChunk.h"

class BaseMapGenerator : public MapGenerator
{
public:
  BaseMapGenerator();
  virtual ~BaseMapGenerator();
  void setup(std::vector<std::vector<float>> &shoreMap);
  std::vector<TerrainChunk>& getCellChunks();
  GLuint& getCellVAO();
  GLuint& getCellDIBO();
private:
  void generateMap(std::vector<std::vector<float>> &shoreMap);
  void splitChunks(int chunkSize);
  void splitCellChunks(int chunkSize);
  void fillBufferData();
  void fillCellBufferData();
  void bufferData(GLuint& ebo, GLuint& vbo, GLfloat *buffer, size_t size);
  void setupGLBufferAttributes();
  void setupGLBufferInstancedAttributes();
  GLuint squareModelVbo;
  GLuint cellVao, cellVbo, cellEbo, cellModelVbo, cellIndirectDrawCommandBO;
  std::vector<std::vector<float>> chunkMap;
  std::vector<TerrainTile> cellTiles;
  std::default_random_engine randomizer;
  std::vector<TerrainChunk> chunks;
  std::vector<TerrainChunk> cellChunks;
};

#endif // BASEMAPGENERATOR_H
