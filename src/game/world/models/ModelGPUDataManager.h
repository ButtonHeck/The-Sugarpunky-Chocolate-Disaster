#ifndef MODELGPUDATAMANAGER_H
#define MODELGPUDATAMANAGER_H
#include <GL/glew.h>
#include <glm/gtx/norm.hpp>
#include "game/world/models/Mesh.h"
#include "game/world/models/ModelChunk.h"
#include "graphics/openglObjects/BufferCollection.h"

class ModelGPUDataManager
{
public:
  ModelGPUDataManager(bool isParentModelLowPoly);
  void setupBuffers(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices);
  void prepareIndirectBufferData(const std::vector<ModelChunk>& chunks,
                                 unsigned int modelIndex,
                                 const glm::vec2& cameraPositionXZ,
                                 const Frustum &frustum,
                                 float loadingDistance,
                                 float loadingDistanceShadow,
                                 float loadingDistanceLowPoly);
  void updateIndirectBufferData();
  void loadModelInstances(const std::vector<glm::mat4> &instanceMatrices, unsigned int numInstances);
  void cleanup();
  GLsizei getPrimitiveCount(bool isShadow) const;
  BufferCollection &getBasicGLBuffers();
  BufferCollection &getShadowDIBO();

private:
  constexpr static int NUM_CHUNKS = (WORLD_WIDTH / CHUNK_SIZE) * (WORLD_HEIGHT / CHUNK_SIZE);
  constexpr static size_t INDIRECT_DRAW_COMMAND_ARGUMENTS = 5;
  struct IndirectBufferToken
  {
    //{ indicesCount, numInstancesToDraw, firstIndex, baseVertex, baseInstance }
    IndirectBufferToken(GLuint indicesCount, GLuint numInstances, GLuint instanceOffset);
    GLuint indicesCount, numInstances, instanceOffset;
    static const GLuint FIRST_INDEX = 0, BASE_VERTEX = 0;
  };
  void addIndirectBufferData(int distanceToChunk, GLuint indicesSize, GLuint numInstances, GLuint instanceOffset, bool isShadow);

  //parent model attributes
  GLuint indicesSize;
  bool isLowPoly;

  //screen rendering related variables
  BufferCollection basicGLBuffers;
  GLuint multiDrawIndirectData[NUM_CHUNKS * INDIRECT_DRAW_COMMAND_ARGUMENTS] = {0};
  /*
  a multimap of approximated "distance-to-chunk" as a key and IndirectBufferToken as a value
  has been chosen because after all necessary insertions we have indirect draw commands
  (each of which is an instance of the IndirectBufferToken class) sorted in the ascending order,
  thus we have roughly front to back order of model instances rendering
  */
  std::multimap<int, IndirectBufferToken> indirectTokens;
  GLsizei drawIndirectCommandPrimCount = 0;

  //shadow rendering related variables
  BufferCollection shadowDIBO;
  GLuint multiDrawIndirectDataShadow[NUM_CHUNKS * INDIRECT_DRAW_COMMAND_ARGUMENTS] = {0};
  std::multimap<int, IndirectBufferToken> indirectTokensShadow;
  GLsizei drawIndirectCommandPrimCountShadow = 0;
};

#endif // MODELGPUDATAMANAGER_H
