#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H
#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/shaders/Shader.h"
#include "graphics/textures/TextureUnits.h"
#include "graphics/shaders/ShaderUnits.h"
#include "util/BenchmarkTimer.h"
#include "game/world/SceneSettings.h"
#include "graphics/ScreenResolution.h"

class ShaderManager
{
public:
  ShaderManager();
  virtual ~ShaderManager();
  void setupConstantUniforms(const ScreenResolution &screenResolution);
  Shader& get(SHADER type);
private:
  std::unordered_map<int, Shader> shaders;
};

#endif // SHADERMANAGER_H
