#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H
#include <vector>
#include "graphics/Shader.h"
#include "graphics/TextureUnits.h"
#include "graphics/Frustum.h"

enum SHADER
{
  SHADER_HILLS_CULLING = 0,
  SHADER_HILLS = 1,
  SHADER_SHORE = 2,
  SHADER_UNDERWATER = 3,
  SHADER_FLAT = 4,
  SHADER_WATER_CULLING = 5,
  SHADER_WATER = 6,
  SHADER_SKY = 7,
  SHADER_MODELS = 8,
  SHADER_FONT = 9,
  SHADER_CS = 10,
  SHADER_BUILDABLE = 11,
  SHADER_SELECTED = 12,
  SHADER_MS_TO_DEFAULT = 13,
  SHADER_SHADOW_TERRAIN = 14,
  SHADER_SHADOW_MODELS = 15,
  SHADER_SHADOW_TERRAIN_CAMERA = 16,
  SHADER_SHADOW_MODELS_CAMERA = 17,
  SHADER_MODELS_PHONG = 18
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
  std::vector<std::pair<SHADER, Shader>> shaders;
};

#endif // SHADERMANAGER_H
