#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H
#include <unordered_map>
#include "graphics/Shader.h"
#include "graphics/TextureUnits.h"
#include "graphics/Frustum.h"

enum SHADER
{
  SHADER_HILLS_CULLING = 0,
  SHADER_HILLS,
  SHADER_SHORE,
  SHADER_UNDERWATER,
  SHADER_LAND,
  SHADER_WATER_CULLING,
  SHADER_WATER,
  SHADER_SKYBOX,
  SHADER_MODELS,
  SHADER_MODELS_PHONG,
  SHADER_FONT,
  SHADER_COORDINATE_SYSTEM,
  SHADER_BUILDABLE,
  SHADER_SELECTED,
  SHADER_MS_TO_DEFAULT,
  SHADER_SHADOW_TERRAIN,
  SHADER_SHADOW_MODELS,
  SHADER_SHADOW_TERRAIN_CAMERA,
  SHADER_SHADOW_MODELS_CAMERA
};

class ShaderManager
{
public:
  ShaderManager();
  virtual ~ShaderManager();
  void setupConstantUniforms(glm::mat4 fontProjection);
  Shader& get(SHADER type);
  void updateHillsShaders(bool useFC, bool useShadows, glm::mat4& projectionView, glm::vec3& viewPosition, Frustum& viewFrustum, float maxHillHeight);
  void updateShoreShader(glm::mat4& projectionView, bool useShadows);
  void updateFlatShader(glm::mat4& projectionView, bool useShadows);
  void updateUnderwaterShader(glm::mat4& projectionView);
  void updateBuildableShader(glm::mat4& projectionView);
  void updateSelectedShader(glm::mat4& projectionView, glm::mat4& selectedModel);
  void updateWaterShaders(bool useFC, glm::mat4& projectionView, glm::vec3& viewPosition, Frustum& viewFrustum);
  void updateSkyShader(glm::mat4 &projectionView);
  void updateModelShader(glm::mat4& projectionView, glm::vec3& viewPosition,
                         bool shadowOnTrees,
                         bool useShadows,
                         bool useFlatBlending);
private:
  std::unordered_map<int, Shader> shaders;
};

#endif // SHADERMANAGER_H
