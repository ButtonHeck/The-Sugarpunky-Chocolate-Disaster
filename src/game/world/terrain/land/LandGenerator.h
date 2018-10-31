#ifndef LANDGENERATOR_H
#define LANDGENERATOR_H
#include <random>
#include <chrono>
#include <memory>
#include "game/world/terrain/Generator.h"
#include "game/world/terrain/land/LandChunk.h"

class LandGenerator : public Generator
{
public:
  LandGenerator();
  virtual ~LandGenerator() = default;
  void setup(map2D_f &shoreMap);

private:
  friend class LandRenderer;
  friend class LandFacade;

  void generateMap(map2D_f &shoreMap);
  void splitChunks(int chunkSize);
  void splitCellChunks(int chunkSize);
  void fillBufferData();
  void fillCellBufferData();
  void bufferData(GLuint& ebo, GLuint& vbo, GLfloat *buffer, size_t size);
  void setupGLBufferAttributes();
  void setupGLBufferInstancedAttributes();

  OpenglBuffer cellBuffers;
  map2D_f chunkMap;
  std::vector<TerrainTile> cellTiles;
  std::default_random_engine randomizer;
  std::vector<LandChunk> chunks;
  std::vector<LandChunk> cellChunks;
};

#endif // LANDGENERATOR_H
