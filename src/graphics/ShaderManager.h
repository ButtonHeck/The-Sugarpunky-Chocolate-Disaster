#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H
#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/Shader.h"
#include "graphics/TextureUnits.h"
#include "graphics/ShaderUnits.h"
#include "util/BenchmarkTimer.h"
#include "util/SceneSettings.h"
#include "graphics/ScreenResolution.h"

class ShaderManager
{
public:
  ShaderManager();
  virtual ~ShaderManager();
  void setupConstantUniforms(ScreenResolution &screenResolution);
  Shader& get(SHADER type);
private:
  std::unordered_map<int, Shader> shaders;
};

#endif // SHADERMANAGER_H
