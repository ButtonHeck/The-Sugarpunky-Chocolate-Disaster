#pragma once

#include "Generator"

#include <memory>

class WaterShader;

class WaterGenerator : public Generator
{
public:
  WaterGenerator(WaterShader& shaders);
  void setup();
  void setupConsiderTerrain();

private:
  constexpr static unsigned int RIVER_SIZE_TO_INCREASE_COUNTER = 19;
  constexpr static unsigned int RIVER_DIRECTION_CHANGE_DELAY = 48;

  friend class WaterRenderer;
  struct WaterVertex
  {
    constexpr static unsigned int NUMBER_OF_ELEMENTS = 3;
    WaterVertex(glm::vec3 position) noexcept;
    float posX, posY, posZ;
  };
  enum DIRECTION : int {
      UP = 0, UP_RIGHT, RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT, UP_LEFT, NUM_DIRECTIONS
  };

  void generateMap();
  void addWaterNearbyTerrain();
  void setNewDirection(unsigned int &curveDistanceStep,
                       unsigned int &curveMaxDistance,
                       DIRECTION &currentDirection,
                       DIRECTION validDirectionLeft,
                       DIRECTION validDirectionRight) noexcept;
  void fattenKernel(int x, int y, int &riverTileCounter, int &riverWidthOffset, bool &riverWidthIncrease);
  void bufferVertex(GLfloat* vertices, int offset, WaterVertex vertex) noexcept;
  void setupGLBufferAttributes() noexcept;
  void fillBufferData();

  BufferCollection culledBuffers;
  WaterShader& shaders;
  size_t numVertices;
  size_t numTiles;
  std::unique_ptr<GLfloat[]> vertices;
  map2D_f postProcessMap;
};
