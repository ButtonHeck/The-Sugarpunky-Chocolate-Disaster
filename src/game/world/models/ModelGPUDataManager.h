#ifndef MODELGPUDATAMANAGER_H
#define MODELGPUDATAMANAGER_H
#include <GL/glew.h>
#include <memory>
#include "Mesh.h"
#include "ModelChunk.h"
#include "../../src/graphics/openglObjects/BufferCollection.h"

class ModelGPUDataManager
{
public:
  ModelGPUDataManager(bool isParentModelLowPoly);
  void setupBuffers(const std::vector<Mesh::Vertex> &vertices, const std::vector<GLuint> &indices, bool useIndirectBuffer);
  void prepareIndirectBufferData(const std::vector<std::pair<ModelChunk, unsigned int> > &chunks,
                                 unsigned int modelIndex,
                                 float loadingDistance,
                                 float loadingDistanceShadow);
  void updateIndirectBufferData();
  void loadModelInstances(const std::vector<glm::mat4> &instanceMatrices, unsigned int numInstances);
  GLsizei getPrimitiveCount(bool isShadow) const;
  BufferCollection &getBasicGLBuffers();
  BufferCollection &getShadowDIBO();

private:
  constexpr static int NUM_CHUNKS = (WORLD_WIDTH / CHUNK_SIZE) * (WORLD_HEIGHT / CHUNK_SIZE);

  struct IndirectBufferToken
  {
    //{ indicesCount, numInstancesToDraw, firstIndex, baseVertex, baseInstance }
    IndirectBufferToken(GLuint numInstances, GLuint instanceOffset);
    GLuint numInstances, instanceOffset;
    static const GLuint FIRST_INDEX = 0, BASE_VERTEX = 0;
  };

  void setupIndirectBuffer();
  void addIndirectBufferData(GLuint numInstances, GLuint instanceOffset, bool isShadow);

  //parent model attributes
  GLuint indicesCount;
  bool isLowPoly;

  //screen rendering related variables
  BufferCollection basicGLBuffers;
  std::unique_ptr<GLuint[]> multiDrawIndirectData;
  std::vector<IndirectBufferToken> indirectTokens;
  GLsizei drawIndirectCommandPrimCount = 0;

  //shadow rendering related variables
  BufferCollection shadowDIBO;
  std::unique_ptr<GLuint[]> multiDrawIndirectDataShadow;
  std::vector<IndirectBufferToken> indirectTokensShadow;
  GLsizei drawIndirectCommandPrimCountShadow = 0;
};

#endif // MODELGPUDATAMANAGER_H
