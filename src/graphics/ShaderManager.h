#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H
#include <vector>
#include <glm/detail/func_geometric.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "Shader.h"
#include "src/game/Settings.h"
#include "Frustum.h"

enum SHADER_TYPE
{
  SHADER_HILLS = 0,
  SHADER_HILLS_NOFC = 1,
  SHADER_SHORE = 2,
  SHADER_UNDERWATER = 3,
  SHADER_FLAT = 4,
  SHADER_WATER = 5,
  SHADER_WATER_NOFC = 6,
  SHADER_SKY = 7,
  SHADER_MODELS = 8,
  SHADER_FONT = 9,
  SHADER_CS = 10,
  SHADER_BUILDABLE = 11,
  SHADER_SELECTED = 12,
  SHADER_MS_TO_DEFAULT = 13,
  SHADER_HDR = 14,
  SHADER_SHADOW_FLAT = 15,
  SHADER_SHADOW_HILLS = 16,
  SHADER_SHADOW_MODELS = 17,
  SHADER_SHADOW_SHORE = 18,
  SHADER_SHADOW_UNDERWATER = 19,
  SHADER_SHADOW_WATER = 20
};

class ShaderManager
{
public:
  ShaderManager();
  void setupConstantUniforms();
  Shader& get(SHADER_TYPE type);
  void updateHillsShaders(bool enableFC, glm::mat4& projectionView, glm::vec3& viewPosition, Frustum& viewFrustum);
  void updateShoreShader(glm::mat4& projectionView);
  void updateFlatShader(glm::mat4& projectionView);
  void updateUnderwaterShader(glm::mat4& projectionView);
  void updateBuildableShader(glm::mat4& projectionView);
  void updateSelectedShader(glm::mat4& projectionView, glm::mat4& selectedModel);
  void updateWaterShaders(bool enableFC, glm::mat4& projectionView, glm::vec3& viewPosition, Frustum& viewFrustum);
  void updateSkyShader(glm::mat4& view, glm::mat4& projection);
  void updateModelShader(glm::mat4& projectionView, glm::vec3& viewPosition, bool shadowOnTrees);
  void deleteShaders();
private:
  std::vector<std::pair<SHADER_TYPE, Shader>> shaders;
};

#endif // SHADERMANAGER_H
