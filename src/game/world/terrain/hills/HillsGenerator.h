#ifndef HILLSGENERATOR_H
#define HILLSGENERATOR_H
#include "game/world/terrain/Generator.h"
#include "game/world/terrain/hills/HillsShader.h"
#include <random>
#include <chrono>

namespace HILL_DENSITY
{
  constexpr float HILLS_THIN = 3.1f * (float)WORLD_WIDTH;
  constexpr float HILLS_MEDIUM = 3.0f * (float)WORLD_WIDTH;
  constexpr float HILLS_DENSE = 2.9f * (float)WORLD_WIDTH;
}

class HillsGenerator : public Generator
{
public:
  HillsGenerator(HillsShader& shaders, const map2D_f& waterMap);
  virtual ~HillsGenerator() = default;
  void setup();
  void createTilesAndBufferData();

private:
  friend class HillsRenderer;
  friend class HillsFacade;
  struct HillVertex
  {
    HillVertex(glm::vec3 pos, glm::vec2 texCoords, glm::vec3 normal, glm::vec3 tangent, glm::vec3 bitangent);
    GLfloat posX, posY, posZ;
    GLfloat texCoordX, texCoordY;
    GLfloat normalX, normalY, normalZ;
    GLfloat tangentX, tangentY, tangentZ;
    GLfloat bitangentX, bitangentY, bitangentZ;
  };

  void generateMap(int cycles, float density);
  void generateKernel(int cycles, float density);
  void fattenKernel(int cycles);
  void bufferVertex(GLfloat* vertices, int offset, HillVertex vertex);
  void fillBufferData();
  void setupGLBufferAttributes();
  bool hasWaterNearby(int x, int y, int radius);
  void compressMap(float thresholdPercent, float ratio);
  void updateMaxHeight();
  void removeMapPlateaus(float plateauHeight);
  void removeOrphanHills();
  void smoothMapSinks();
  void generateTangentMap();
  void generateBitangentMap();

  BufferCollection culledBuffers;
  HillsShader& shaders;
  float maxHeight = 0.0f;
  const map2D_f& waterMap;
  map2D_vec3 normalMap;
  map2D_vec3 tangentMap;
  map2D_vec3 bitangentMap;
  std::default_random_engine randomizer;
};

#endif // HILLSGENERATOR_H
