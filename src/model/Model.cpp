#include "model/Model.h"
#include <IL/il.h>

Model::Model(const std::string& path)
{
  loadModel(std::string(MODELS_DIR + path));
}

Model::Model(Model &&old) noexcept
  :
    meshes(old.meshes),
    textures_loaded(old.textures_loaded),
    directory(old.directory)
{}

void Model::cleanup()
{
  for (Mesh& mesh : meshes)
    mesh.cleanup();
}

TextureLoader* Model::textureLoader;
void Model::bindTextureLoader(TextureLoader &textureLoader)
{
  Model::textureLoader = &textureLoader;
}

void Model::loadModel(const std::string &path)
{
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, aiProcess_CalcTangentSpace | aiProcess_FlipUVs | aiProcess_Triangulate);
  if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
    Logger::log("Error while loading Assimp: %\n", importer.GetErrorString());
  directory = path.substr(0, path.find_last_of('/'));
  processNode(scene->mRootNode, scene);
}

void Model::draw(bool isShadow)
{
  for (unsigned int i = 0; i < meshes.size(); i++)
    meshes[i].draw(isShadow);
}

void Model::prepareMeshesIndirectData(std::vector<ModelChunk>& chunks,
                                      unsigned int index,
                                      const glm::vec2& cameraPositionXZ,
                                      const Frustum &frustum)
{
  for (unsigned int i = 0; i < meshes.size(); i++)
    meshes[i].prepareIndirectBufferData(chunks, index, cameraPositionXZ, frustum);
}

void Model::updateIndirectBufferData()
{
  for (unsigned int i = 0; i < meshes.size(); ++i)
    meshes[i].updateIndirectBufferData();
}

void Model::processNode(aiNode *node, const aiScene* scene)
{
  for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.emplace_back(std::move(processMesh(mesh, scene)));
    }
  for (unsigned int i = 0; i < node->mNumChildren; i++)
    processNode(node->mChildren[i], scene);
}

void Model::loadInstances(glm::mat4 *models, unsigned int numModels)
{
  for (Mesh& mesh : meshes)
    mesh.setupInstances(models, numModels);
}

std::vector<Mesh> &Model::getMeshes()
{
  return meshes;
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene* scene)
{
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  std::vector<Texture> textures;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
      Vertex vertex;
      glm::vec3 vector;
      //position
      vector.x = mesh->mVertices[i].x;
      vector.y = mesh->mVertices[i].y;
      vector.z = mesh->mVertices[i].z;
      vertex.Position = vector;
      //normals
      vector.x = mesh->mNormals[i].x;
      vector.y = mesh->mNormals[i].y;
      vector.z = mesh->mNormals[i].z;
      vertex.Normal = vector;
      //TexCoords (if any)
      if (mesh->mTextureCoords[0])
        {
          glm::vec2 texVec;
          texVec.x = mesh->mTextureCoords[0][i].x;
          texVec.y = mesh->mTextureCoords[0][i].y;
          vertex.TexCoords = texVec;
        }
      else
        vertex.TexCoords = glm::vec2(0.0, 0.0);
      //tangent
      vector.x = mesh->mTangents[i].x;
      vector.y = mesh->mTangents[i].y;
      vector.z = mesh->mTangents[i].z;
      vertex.Tangent = vector;
      //bitangent
      vector.x = mesh->mBitangents[i].x;
      vector.y = mesh->mBitangents[i].y;
      vector.z = mesh->mBitangents[i].z;
      vertex.Bitangent = vector;
      vertices.emplace_back(std::move(vertex));
    }
  //process indices
  for (unsigned int i=0; i < mesh->mNumFaces; i++)
    {
      aiFace face = mesh->mFaces[i];
      for (unsigned int j = 0; j < face.mNumIndices; j++)
        indices.emplace_back(std::move(face.mIndices[j]));
    }
  //process materials
  aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

  std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "u_texture_diffuse");
  textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

  std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "u_texture_specular");
  textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

  std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "u_texture_normal");
  textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

  std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "u_texture_height");
  textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

  return Mesh(vertices, textures, indices);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *material, aiTextureType type, std::__cxx11::string typeName)
{
  std::vector<Texture> textures;
  for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
    {
      bool skip = false;
      aiString texturePath;
      material->GetTexture(type, i, &texturePath);
      for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
          if (strcmp(texturePath.C_Str(), textures_loaded[j].path.data()) == 0)
            {
              skip = true;
              break;
            }
        }
      if (!skip)
        {
          Texture texture;
          texture.type = typeName;
          std::string path = this->directory + '/' + std::string(texturePath.C_Str());
          texture.id = textureLoader->loadTexture(path, TEX_MESH_DIFFUSE + i, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true, !INCLUDE_RES_DIR);
          texture.path = texturePath.C_Str();
          textures.emplace_back(texture);
          textures_loaded.emplace_back(texture);
        }
    }
  return textures;
}
