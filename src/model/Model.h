#ifndef MODEL_H
#define MODEL_H
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>
#include <glm/gtx/norm.hpp>
#include "model/Mesh.h"
#include "graphics/TextureLoader.h"
#include "graphics/TextureUnits.h"
#include "game/world/plants/ModelChunk.h"
#include "util/Settings.h"
#include "graphics/Frustum.h"
#include "graphics/OpenglBuffer.h"
#include "graphics/BindlessTextureManager.h"

class Model
{
public:
  Model(const std::string& path, bool isLowPoly);
  static void bindTextureLoader(TextureLoader& textureLoader);
  void draw(bool isShadow);
  void prepareMeshesIndirectData(std::vector<ModelChunk>& chunks,
                                 unsigned int index,
                                 const glm::vec2& cameraPositionXZ,
                                 const Frustum& frustum,
                                 float loadingDistance,
                                 float loadingDistanceShadow,
                                 float loadingDistanceLowPoly);
  void updateIndirectBufferData();
  void loadModelInstances(glm::mat4* models, unsigned int numModels);
  void cleanup();

private:
  void loadModel(const std::string& path);
  void processNode(aiNode* node, const aiScene* scene, GLuint &meshIndexOffset);
  Mesh processMesh(aiMesh* mesh, const aiScene* scene, GLuint meshIndexOffset);
  void loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName, unsigned int &textureIndex);
  void setup();
  constexpr static int NUM_CHUNKS = (WORLD_WIDTH / CHUNK_SIZE) * (WORLD_HEIGHT / CHUNK_SIZE);

  struct IndirectBufferToken
  {
    IndirectBufferToken(GLuint indicesCount, GLuint numInstances, GLuint instanceOffset);
    //{ indicesCount, numInstancesToDraw, firstIndex, baseVertex, baseInstance }
    GLuint indicesCount, numInstances, instanceOffset;
    static const GLuint FIRST_INDEX = 0, BASE_VERTEX = 0;
  };
  void addIndirectBufferData(int directionToChunkLength, GLuint indicesSize, GLuint numInstances, GLuint instanceOffset, bool shadow);

  std::vector<Mesh> meshes;
  std::string directory;
  static TextureLoader* textureLoader;

  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  bool isLowPoly;

  //screen rendering related variables
  OpenglBuffer basicGLBuffers;
  GLuint multiDrawIndirectData[NUM_CHUNKS * 5] = {0};
  std::multimap<int, IndirectBufferToken> indirectTokensSorted;
  size_t drawIndirectCommandPrimCount = 0;

  //shadow rendering related variables
  OpenglBuffer shadowDIBO;
  GLuint multiDrawIndirectDataShadow[NUM_CHUNKS * 5] = {0};
  std::multimap<int, IndirectBufferToken> indirectTokensSortedShadow;
  size_t drawIndirectCommandPrimCountShadow = 0;
};

#endif // MODEL_H
