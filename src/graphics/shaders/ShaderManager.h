#pragma once

#include "Shader"
#include "TextureUnits"
#include "ShaderUnits"
#include "SceneSettings"
#include "ScreenResolution"

#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>

class ShaderManager
{
public:
  ShaderManager();
  virtual ~ShaderManager();
  void setupConstantUniforms(const ScreenResolution &screenResolution);
  Shader& get(SHADER_UNITS type);

private:
  std::unordered_map<int, Shader> shaders;
};
