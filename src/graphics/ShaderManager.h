#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H
#include <unordered_map>
#include "graphics/Shader.h"
#include "graphics/TextureUnits.h"
#include "graphics/ShaderUnits.h"
#include "util/BenchmarkTimer.h"
#include "util/SceneSettings.h"

class ShaderManager
{
public:
  ShaderManager();
  virtual ~ShaderManager();
  void setupConstantUniforms(glm::mat4 fontProjection);
  Shader& get(SHADER type);
private:
  std::unordered_map<int, Shader> shaders;
};

#endif // SHADERMANAGER_H
