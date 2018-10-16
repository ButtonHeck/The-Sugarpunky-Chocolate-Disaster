#ifndef LANDGENERATOR_H
#define LANDGENERATOR_H
#include <random>
#include <chrono>
#include <memory>
#include "game/world/terrain/Generator.h"
#include "game/world/terrain/TerrainChunk.h"

class LandGenerator : public Generator
{
public:
  LandGenerator();
  virtual ~LandGenerator() = default;
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
  OpenglBuffer cellBuffers;
  std::vector<std::vector<float>> chunkMap;
  std::vector<TerrainTile> cellTiles;
  std::default_random_engine randomizer;
  std::vector<TerrainChunk> chunks;
  std::vector<TerrainChunk> cellChunks;
};

#endif // LANDGENERATOR_H
