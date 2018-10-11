#ifndef SHOREGENERATOR_H
#define SHOREGENERATOR_H
#include <random>
#include <chrono>
#include <memory>
#include "generator/MapGenerator.h"

class ShoreGenerator : public MapGenerator
{
public:
  ShoreGenerator(std::vector<std::vector<float>>& waterMap);
  ~ShoreGenerator() = default;
  void setup();
private:
  struct ShoreVertex
  {
    ShoreVertex(glm::vec3 position, glm::vec2 texCoords, glm::vec3 normal);
    float posX, posY, posZ;
    float texCoordX, texCoordY;
    float normalX, normalY, normalZ;
  };
  void generateMap();
  void smoothMap();
  void randomizeShore();
  void correctMapAtEdges();
  void compressMap(float ratio);
  void removeUnderwaterTiles(float thresholdValue);
  void createTiles();
  void fillBufferData();
  void bufferVertex(GLfloat* vertices, int offset, ShoreVertex vertex);
  std::vector<std::vector<float>>& waterMap;
  std::vector<std::vector<glm::vec3>> normalMap;
  std::default_random_engine randomizer;
};

#endif // SHOREGENERATOR_H
