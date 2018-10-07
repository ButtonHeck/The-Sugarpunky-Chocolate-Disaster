#ifndef MESH_H
#define MESH_H
#include <vector>
#include <cstring>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/detail/func_common.hpp>
#include <glm/detail/func_geometric.hpp>
#include <glm/gtx/norm.hpp>
#include "graphics/Shader.h"
#include "game/Settings.h"
#include "chunk/ModelChunk.h"
#include "graphics/Camera.h"
#include "graphics/Frustum.h"
#include "game/Settings.h"
#include "timer/BenchmarkTimer.h"
#include "model/IndirectBufferToken.h"
#include "graphics/TextureUnits.h"
#include <map>

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
  void cleanup();
  void setupMesh();
  void setupInstances(glm::mat4* models, unsigned int numModels);
  void draw(bool useCulling, bool bindTexture, bool updateIndirect);
  void prepareIndirectBufferData(std::vector<ModelChunk>& chunks,
                                 unsigned int index,
                                 const glm::vec2 &cameraPositionXZ,
                                 const Frustum& frustum);
private:
  void addIndirectBufferData(int directionToChunkLength, GLuint indicesSize, GLuint numInstances, GLuint instanceOffset);
  std::vector<Vertex> vertices;
  std::vector<Texture> textures;
  std::vector<GLuint> indices;
  unsigned int VAO, VBO, EBO, instanceVBO = 0, multiDrawIndirectBO;
  unsigned int numInstances;
  constexpr static int NUM_CHUNKS = (WORLD_WIDTH / CHUNK_SIZE) * (WORLD_HEIGHT / CHUNK_SIZE);
  GLuint multiDrawIndirectData[NUM_CHUNKS * 5] = {0}; //{ indicesCount, numInstancesToDraw, firstIndex, baseVertex, baseInstance }
  std::multimap<int, IndirectBufferToken> indirectTokensSorted;
  GLuint drawIndirectCommandPrimCount = 0;
};

#endif // MESH_H
