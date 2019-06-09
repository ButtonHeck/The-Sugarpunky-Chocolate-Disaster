/*
 * Copyright 2019 Ilya Malgin
 * Model.h
 * This file is part of The Sugarpunky Chocolate Disaster project
 *
 * The Sugarpunky Chocolate Disaster project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The Sugarpunky Chocolate Disaster project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * See <http://www.gnu.org/licenses/>
 *
 * Purpose: contains declaration for Model class
 * @version 0.1.0
 */

#pragma once

#include "Mesh"
#include "ModelGPUDataManager"
#include "ModelRenderer"

#include <assimp/scene.h>

class TextureLoader;
class ModelChunk;

/**
 * @brief Wrapper for .obj model.
 * Responsible for compiling model's meshes data into one contiguous storage and delegating tasks
 * among its member objects
 */
class Model
{
public:
  Model(const std::string& path, bool isLowPoly, unsigned int numRepetitions = 1, bool isInstanced = true);
  static void bindTextureLoader(TextureLoader& textureLoader);
  void draw(bool isShadow);
  void drawOneInstance();
  void prepareIndirectBufferData(const std::vector<std::pair<ModelChunk, unsigned int> > &visibleChunks,
                                 unsigned int modelIndex,
                                 float loadingDistance,
                                 float loadingDistanceShadow);
  void updateIndirectBufferData();
  void loadModelInstances(const std::vector<glm::mat4> &instanceMatrices, unsigned int numInstances);
  unsigned int getRepeatCount() const;

private:
  static TextureLoader* textureLoader;
  void load(const std::string& path);
  void processNode(const aiNode *node, const aiScene* scene, GLuint &meshVertexIndexOffset);
  void loadMaterialTextures(const aiMaterial *material, aiTextureType type, const std::string &typeName, unsigned int &textureIndex);

  std::string directory;
  std::vector<Mesh::Vertex> vertices;
  std::vector<GLuint> indices;
  bool isInstanced;
  bool isLowPoly;
  unsigned int numRepetitions;
  ModelGPUDataManager GPUDataManager;
  ModelRenderer renderer;
};
