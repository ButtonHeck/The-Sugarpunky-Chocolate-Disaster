#ifndef WATERMAPGENERATOR_H
#define WATERMAPGENERATOR_H
#include "MapGenerator.h"
#include <GLFW/glfw3.h>
#include <memory>
#include "game/Options.h"
#include "game/Settings.h"
#include "timer/BenchmarkTimer.h"
#include "graphics/Shader.h"

class WaterMapGenerator : public MapGenerator
{
public:
  WaterMapGenerator(Shader& waterShader);
  ~WaterMapGenerator();
  void setup();
  void setupConsiderTerrain();
  void fillBufferData();
  void updateVerticesBuffer();
  void updateAnimationFrame(Options& options);
  GLuint getCulledVAO() const;
  GLuint getTransformFeedback() const;
private:
  struct WaterVertex
  {
    WaterVertex(glm::vec3 position, glm::vec3 normal);
    float posX, posY, posZ;
    float normalX, normalY, normalZ;
  };
  void bufferVertex(GLfloat* vertices, int offset, WaterVertex vertex);
  void setupGLBufferAttributes();
  GLuint culledVAO = 0, culledVBO = 0, TFBO = 0;
  Shader& waterShader;
  size_t numVertices;
  std::unique_ptr<GLfloat[]> vertices;
  constexpr static int WATER_HEIGHT_OFFSETS_SIZE = NUM_TILES + WORLD_WIDTH * 2;
  std::unique_ptr<GLfloat[]> waterHeightOffsets;
  void generateMap(int shoreSizeBase, unsigned int &numWaterTiles);
  void addWaterNearbyTerrain();
  enum DIRECTION : int {
      UP = 0, UP_RIGHT, RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT, UP_LEFT
  };
  void setNewDirection(int x, int y,
                unsigned int& numTiles,
                int &numCurveChanges,
                unsigned int &curveDistanceStep,
                unsigned int &curveMaxDistance,
                DIRECTION &dir, DIRECTION limit1, DIRECTION limit2);
  void fattenKernel(int x, int y,
                    int shoreSizeBase,
                    unsigned int& numTiles,
                    int &riverTileCounter,
                    int &shoreSizeOffset,
                    bool &shoreSizeIncrease);
  std::vector<std::vector<float>> postProcessMap;
};

#endif // WATERMAPGENERATOR_H
