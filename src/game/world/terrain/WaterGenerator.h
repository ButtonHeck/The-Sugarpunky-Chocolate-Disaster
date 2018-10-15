#ifndef WATERGENERATOR_H
#define WATERGENERATOR_H
#include "game/world/terrain/Generator.h"
#include <GLFW/glfw3.h>
#include <memory>
#include "game/Options.h"
#include "util/Settings.h"
#include "util/BenchmarkTimer.h"
#include "graphics/Shader.h"

class WaterGenerator : public Generator
{
public:
  WaterGenerator(Shader& shader);
  virtual ~WaterGenerator();
  void setup();
  void setupConsiderTerrain();
  void bufferNewData();
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
  void updateVertexNormal(GLfloat* vertices, int offset, glm::vec3 normal);
  void updateTileY(GLfloat* vertices, int offset, glm::vec4 heights);
  void setupGLBufferAttributes();
  void fillBufferData();
  GLuint culledVAO = 0, culledVBO = 0, TFBO = 0;
  Shader& shader;
  size_t numVertices;
  unsigned int numTiles;
  std::unique_ptr<GLfloat[]> vertices;
  constexpr static int WATER_HEIGHT_OFFSETS_SIZE = NUM_TILES + WORLD_WIDTH * 2;
  std::unique_ptr<GLfloat[]> heightOffsets;
  void generateMap();
  void addWaterNearbyTerrain();
  enum DIRECTION : int {
      UP = 0, UP_RIGHT, RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT, UP_LEFT
  };
  void setNewDirection(int x, int y,
                int &numCurveChanges,
                unsigned int &curveDistanceStep,
                unsigned int &curveMaxDistance,
                DIRECTION &dir, DIRECTION limit1, DIRECTION limit2);
  void fattenKernel(int x, int y,
                    int &riverTileCounter,
                    int &shoreSizeOffset,
                    bool &shoreSizeIncrease);
  std::vector<std::vector<float>> postProcessMap;
};

#endif // WATERGENERATOR_H
