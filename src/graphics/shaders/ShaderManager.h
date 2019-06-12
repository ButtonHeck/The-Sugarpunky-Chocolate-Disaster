#pragma once

#include "ShaderUnits"

#include <unordered_map>

class ScreenResolution;
class Shader;

class ShaderManager
{
public:
  ShaderManager() noexcept;
  virtual ~ShaderManager();
  void setupConstantUniforms(const ScreenResolution &screenResolution);
  Shader& get(SHADER_UNIT type);

private:
  std::unordered_map<int, Shader> shaders;
};
