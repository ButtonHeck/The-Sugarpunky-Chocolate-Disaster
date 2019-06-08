#ifndef SHOREGENERATOR_H
#define SHOREGENERATOR_H
#include <random>
#include <chrono>
#include <memory>
#include "Generator"

constexpr float SHORE_CLIP_LEVEL = -4.0f;

class ShoreGenerator : public Generator
{
public:
  ShoreGenerator(const map2D_f& waterMap);
  virtual ~ShoreGenerator() = default;
  void setup();

private:
  const float MIN_HEIGHT_KERNEL_OFFSET = 0.9f;
  const float MAX_HEIGHT_KERNEL_OFFSET = 1.1f;
  const float MIN_HEIGHT_RANDOMIZE_OFFSET = -0.24f;
  const float MAX_HEIGHT_RANDOMIZE_OFFSET = 0.24f;
  const float HEIGHT_SMOOTH_OFFSET = 0.25f;

  friend class ShoreRenderer;
  friend class ShoreFacade;
  struct ShoreVertex
  {
    constexpr static unsigned int NUMBER_OF_ELEMENTS = 8;
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

  const map2D_f& waterMap;
  map2D_vec3 normalMap;
  std::default_random_engine randomizer;
};

#endif // SHOREGENERATOR_H
