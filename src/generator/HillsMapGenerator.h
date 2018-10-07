#ifndef HILLSMAPGENERATOR_H
#define HILLSMAPGENERATOR_H
#include "MapGenerator.h"
#include "graphics/Shader.h"
#include <random>
#include <chrono>

namespace HILL_DENSITY
{
  constexpr float HILLS_THIN = 3.1f * (float)WORLD_WIDTH;
  constexpr float HILLS_MEDIUM = 3.0f * (float)WORLD_WIDTH;
  constexpr float HILLS_DENSE = 2.9f * (float)WORLD_WIDTH;
}

class HillsMapGenerator : public MapGenerator
{
public:
  HillsMapGenerator(Shader& shader, std::vector<std::vector<float>>& waterMap);
  ~HillsMapGenerator();
  void setup();
  void createTilesAndBufferData();
  float getMaxHeight() const;
  GLuint getCulledVAO() const;
  GLuint getTransformFeedback() const;
  size_t getVerticesToDraw() const;
private:
  struct Vertex{
    Vertex(glm::vec3 pos, glm::vec2 texCoords, glm::vec3 normal);
    float posX, posY, posZ;
    float texCoordX, texCoordY;
    float normalX, normalY, normalZ;
  };
  GLuint culledVAO = 0, culledVBO = 0, TFBO = 0;
  Shader& shader;
  float maxHeight = 0.0f;
  size_t verticesToDraw = 0;
  std::vector<std::vector<float>>& waterMap;
  std::vector<std::vector<glm::vec3>> normalMap;
  std::default_random_engine randomizer;
  void generateMap(int cycles, float density);
  void generateKernel(int cycles, float density);
  void fattenKernel(int cycles);
  void bufferVertex(GLfloat* vertices, int offset, Vertex vertex);
  void fillBufferData();
  void setupGLBufferAttributes();
  void smoothNormals();
  bool hasWaterNearby(int x, int y, int radius);
  void compressMap(float thresholdPercent, float ratio);
  void updateMaxHeight();
  void removeMapPlateaus(float plateauHeight);
  void smoothMapHeightChunks(float selfWeight, float sideWeight, float diagonalWeight);
  void removeOrphanHills();
  void smoothMapSinks();
};

#endif // HILLSMAPGENERATOR_H
