#ifndef MODEL_H
#define MODEL_H
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "game/world/models/Mesh.h"
#include "graphics/textures/TextureLoader.h"
#include "graphics/textures/TextureUnits.h"
#include "game/world/models/ModelChunk.h"
#include "game/world/models/ModelGPUDataManager.h"
#include "game/world/models/ModelRenderer.h"
#include "graphics/textures/BindlessTextureManager.h"

class Model
{
public:
  Model(const std::string& path, bool isLowPoly, unsigned int numRepetitions = 1);
  static void bindTextureLoader(TextureLoader& textureLoader);
  void draw(bool isShadow);
  void drawOneInstance();
  void prepareIndirectBufferData(const std::vector<ModelChunk> &chunks,
                                 unsigned int modelIndex,
                                 const glm::vec2& cameraPositionXZ,
                                 const Frustum& frustum,
                                 float loadingDistance,
                                 float loadingDistanceShadow,
                                 float loadingDistanceLowPoly);
  void updateIndirectBufferData();
  void loadModelInstances(const glm::mat4 *models, unsigned int numModels);
  unsigned int getRepeatCount() const;
  void cleanup();

private:
  static TextureLoader* textureLoader;
  void loadModel(const std::string& path);
  void processNode(aiNode* node, const aiScene* scene, GLuint &meshVertexIndexOffset);
  Mesh processMesh(aiMesh* mesh, const aiScene* scene, GLuint meshVertexIndexOffset);
  void loadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string &typeName, unsigned int &textureIndex);
  void setup();

  std::vector<Mesh> meshes;
  std::string directory;
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  bool isLowPoly;
  unsigned int numRepetitions;
  ModelGPUDataManager GPUDataManager;
  ModelRenderer renderer;
};

#endif // MODEL_H
