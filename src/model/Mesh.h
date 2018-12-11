#ifndef MESH_H
#define MESH_H
#include <vector>
#include <cstring>
#include <glm/gtx/norm.hpp>
#include "graphics/Shader.h"
#include "util/Settings.h"
#include "game/world/plants/ModelChunk.h"
#include "game/Camera.h"
#include "graphics/Frustum.h"
#include "util/Settings.h"
#include "util/BenchmarkTimer.h"
#include "graphics/TextureUnits.h"
#include <map>
#include "graphics/OpenglBuffer.h"

struct Vertex
{
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
  glm::vec3 Tangent;
  glm::vec3 Bitangent;
};

struct Texture
{
  unsigned int id;
  std::string type;
  std::string path;
};

class Mesh
{
public:
  Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures, std::vector<GLuint> indices);
  Mesh(Mesh&& old) noexcept;
  Mesh(const Mesh& rhs);
  void cleanup();
  void setup();
  void setupInstanceVBO(GLuint &modelInstanceVBO);
  void draw(bool isShadow);
  void prepareIndirectBufferData(std::vector<ModelChunk>& chunks,
                                 unsigned int index,
                                 const glm::vec2 &cameraPositionXZ,
                                 const Frustum& frustum);
  void updateIndirectBufferData();

private:
  struct IndirectBufferToken
  {
    IndirectBufferToken(GLuint indicesCount, GLuint numInstances, GLuint instanceOffset);
    //{ indicesCount, numInstancesToDraw, firstIndex, baseVertex, baseInstance }
    GLuint indicesCount, numInstances, instanceOffset;
    static const GLuint FIRST_INDEX = 0, BASE_VERTEX = 0;
    friend class Mesh;
  };

  void addIndirectBufferData(int directionToChunkLength, GLuint indicesSize, GLuint numInstances, GLuint instanceOffset, bool shadow);
  std::vector<Vertex> vertices;
  std::vector<Texture> textures;
  std::vector<GLuint> indices;
  OpenglBuffer basicGLBuffers;
  constexpr static int NUM_CHUNKS = (WORLD_WIDTH / CHUNK_SIZE) * (WORLD_HEIGHT / CHUNK_SIZE);
  //on screen rendering indirect buffer variables
  GLuint multiDrawIndirectData[NUM_CHUNKS * 5] = {0}; //{ indicesCount, numInstancesToDraw, firstIndex, baseVertex, baseInstance }
  std::multimap<int, IndirectBufferToken> indirectTokensSorted;
  GLuint drawIndirectCommandPrimCount = 0;
  //shadow indirect buffer variables
  OpenglBuffer shadowDIBO;
  GLuint multiDrawIndirectDataShadow[NUM_CHUNKS * 5] = {0};
  std::multimap<int, IndirectBufferToken> indirectTokensSortedShadow;
  GLuint drawIndirectCommandPrimCountShadow = 0;
};

#endif // MESH_H
