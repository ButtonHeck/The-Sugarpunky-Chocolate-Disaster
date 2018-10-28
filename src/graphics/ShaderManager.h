#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H
#include <unordered_map>
#include "graphics/Shader.h"
#include "graphics/TextureUnits.h"
#include "graphics/Frustum.h"
#include "graphics/ShaderUnits.h"

class ShaderManager
{
public:
  ShaderManager();
  virtual ~ShaderManager();
  void setupConstantUniforms(glm::mat4 fontProjection);
  Shader& get(SHADER type);
  void updateFlatShader(glm::mat4& projectionView, bool useShadows);
  void updateUnderwaterShader(glm::mat4& projectionView);
  void updateBuildableShader(glm::mat4& projectionView);
  void updateSelectedShader(glm::mat4& projectionView, glm::mat4& selectedModel);
  void updateWaterShaders(bool useFC, glm::mat4& projectionView, glm::vec3& viewPosition, Frustum& viewFrustum);
  void updateSkyShader(glm::mat4 &projectionView, glm::vec3 &cameraPos, int backgroundIndex, bool isStatic);
  void updateSunShader(glm::mat4 &projectionView, glm::mat4& model);
  void updateModelShader(glm::mat4& projectionView, glm::vec3& viewPosition,
                         bool shadowOnTrees,
                         bool useShadows,
                         bool useFlatBlending);
private:
  std::unordered_map<int, Shader> shaders;
};

#endif // SHADERMANAGER_H
