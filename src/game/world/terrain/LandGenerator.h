#ifndef LANDGENERATOR_H
#define LANDGENERATOR_H
#include <random>
#include <chrono>
#include <memory>
#include "Generator.h"
#include "game/world/terrain/TerrainChunk.h"

class LandGenerator : public Generator
{
public:
  LandGenerator();
  virtual ~LandGenerator();
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

#endif // LANDGENERATOR_H
