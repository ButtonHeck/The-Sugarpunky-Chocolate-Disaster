#ifndef WATERGENERATOR_H
#define WATERGENERATOR_H
#include <memory>
#include "game/world/terrain/Generator.h"
#include "game/Options.h"
#include "util/BenchmarkTimer.h"
#include "game/world/terrain/water/WaterShader.h"

class WaterGenerator : public Generator
{
public:
  WaterGenerator(WaterShader& shaders);
  virtual ~WaterGenerator() = default;
  void setup();
  void setupConsiderTerrain();
  void bufferNewData();
  void updateAnimationFrame(double time, Options& options);

private:
  constexpr static int HEIGHT_OFFSETS_BUFFER_SIZE = NUM_TILES + WORLD_WIDTH * 2;
  constexpr static float HEIGHT_OFFSET_1 = 0.0825f;
  constexpr static float HEIGHT_OFFSET_2 = 0.0889f;
  constexpr static double ANIMATION_SPEED = 0.12;
  constexpr static unsigned int RIVER_SIZE_TO_INCREASE_COUNTER = 19;
  constexpr static unsigned int RIVER_DIRECTION_CHANGE_DELAY = 48;

  friend class WaterRenderer;
  struct WaterVertex
  {
    constexpr static unsigned int NUMBER_OF_ELEMENTS = 6;
    WaterVertex(glm::vec3 position, glm::vec3 normal);
    float posX, posY, posZ;
    float normalX, normalY, normalZ;
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
                       DIRECTION validDirectionRight);
  void fattenKernel(int x, int y, int &riverTileCounter, int &riverWidthOffset, bool &riverWidthIncrease);
  void bufferVertex(GLfloat* vertices, int offset, WaterVertex vertex);
  void updateVertexNormal(GLfloat* vertices, int offset, const glm::vec3 &normal);
  void updateTileY(GLfloat* vertices, int offset, glm::vec4&& heights);
  void setupGLBufferAttributes();
  void fillBufferData();

  BufferCollection culledBuffers;
  WaterShader& shaders;
  size_t numVertices;
  unsigned int numTiles;
  std::unique_ptr<GLfloat[]> vertices;
  std::unique_ptr<GLfloat[]> heightOffsets;
  map2D_f postProcessMap;
  map2D_vec3 normalMap;
};

#endif // WATERGENERATOR_H
