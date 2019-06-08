#pragma once

#include "Generator"
#include "LandChunk"

#include <random>
#include <chrono>

class LandGenerator : public Generator
{
public:
  LandGenerator();
  virtual ~LandGenerator() = default;
  void setup(const map2D_f &shoreMap);
  void updateCellsIndirectBuffer(const Frustum &frustum);

private:
  friend class LandRenderer;
  friend class LandFacade;

  void generateMap(const map2D_f &shoreMap);
  void splitChunks(int chunkSize);
  void splitCellChunks(int chunkSize);
  void fillBufferData();
  void fillCellBufferData();
  void bufferData(BufferCollection &bufferCollection, GLfloat *buffer, size_t size);
  void setupGLBufferAttributes();
  void setupGLBufferInstancedAttributes();
  void addIndirectBufferData(GLuint* buffer, GLuint& dataOffset, GLuint numInstances, GLuint instanceOffset);

  BufferCollection cellBuffers;
  map2D_f chunkMap;
  std::vector<TerrainTile> cellTiles;
  std::default_random_engine randomizer;
  std::vector<LandChunk> chunks;
  std::vector<LandChunk> cellChunks;
  GLuint cellPrimitiveCount;
};
