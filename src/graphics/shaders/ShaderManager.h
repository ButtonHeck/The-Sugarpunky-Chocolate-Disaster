#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H
#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>
#include "../../src/graphics/shaders/Shader.h"
#include "../../src/graphics/textures/TextureUnits.h"
#include "../../src/graphics/shaders/ShaderUnits.h"
#include "../../src/game/world/SceneSettings.h"
#include "../../src/graphics/ScreenResolution.h"

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

#endif // SHADERMANAGER_H
