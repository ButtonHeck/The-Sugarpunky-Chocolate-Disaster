#include "graphics/ShaderManager.h"

ShaderManager::ShaderManager()
{
  shaders[SHADER_HILLS_CULLING] = Shader("hillsFC.vs", "hillsFC.gs", "_FC.fs");
  shaders[SHADER_HILLS] = Shader("hills.vs", "hills.fs");
  shaders[SHADER_SHORE] = Shader("shore.vs", "shore.fs");
  shaders[SHADER_UNDERWATER] = Shader("underwater.vs", "underwater.fs");
  shaders[SHADER_LAND] = Shader("land.vs", "land.fs");
  shaders[SHADER_WATER_CULLING] = Shader("waterFC.vs", "waterFC.gs", "_FC.fs");
  shaders[SHADER_WATER] = Shader("water.vs", "water.fs");
  shaders[SHADER_SKYBOX] = Shader("skybox.vs", "skybox.fs");
  shaders[SHADER_MODELS] = Shader("model.vs", "model.fs");
  shaders[SHADER_FONT] = Shader("font.vs", "font.fs");
  shaders[SHADER_COORDINATE_SYSTEM] = Shader("coordinateSystem.vs", "coordinateSystem.gs", "coordinateSystem.fs");
  shaders[SHADER_BUILDABLE] = Shader("buildableTiles.vs", "buildableTiles.fs");
  shaders[SHADER_SELECTED] = Shader("selectedTile.vs", "selectedTile.fs");
  shaders[SHADER_MS_TO_DEFAULT] = Shader("MS_toDefault.vs", "MS_toDefault_hdr.fs");
  shaders[SHADER_SHADOW_TERRAIN] = Shader("terrain_shadow.vs");
  shaders[SHADER_SHADOW_MODELS] = Shader("model_shadow.vs");
  shaders[SHADER_SHADOW_TERRAIN_CAMERA] = Shader("terrain_shadow.vs");
  shaders[SHADER_SHADOW_MODELS_CAMERA] = Shader("model_shadow.vs");
  shaders[SHADER_MODELS_PHONG] = Shader("modelPhong.vs", "modelPhong.fs");
}

ShaderManager::~ShaderManager()
{
  glUseProgram(0);
  for (unsigned int i = 0; i < shaders.size(); i++)
    shaders[i].cleanUp();
}

#define bindShaderUnit(shader, type) \
  shader = &shaders[type]; \
  shader->use();

void ShaderManager::setupConstantUniforms(glm::mat4 fontProjection)
{
  Shader* shader = nullptr;
  bindShaderUnit(shader, SHADER_HILLS);
  shader->setVec3("u_lightDir", glm::normalize(-LIGHT_DIR_TO));
  shader->setInt("u_flat_diffuse", TEX_LAND_x2);
  shader->setInt("u_flat_diffuse2", TEX_LAND_2_x2);
  shader->setInt("u_hills_diffuse", TEX_HILL);
  shader->setInt("u_hills_diffuse2", TEX_HILL_2);
  shader->setInt("u_hills_specular", TEX_HILL_SPECULAR);
  shader->setInt("u_diffuse_mix_map", TEX_DIFFUSE_MIX_MAP);
  shader->setInt("u_normal_map", TEX_TERRAIN_NORMAL);
  shader->setFloat("u_mapDimension", 1.0f / (float)WORLD_WIDTH);
  shader->setMat4("u_lightSpaceMatrix", LIGHT_SPACE_MATRIX);
  shader->setInt("u_shadowMap", TEX_DEPTH_MAP_SUN);

  bindShaderUnit(shader, SHADER_SHORE);
  shader->setInt("u_flat_diffuse", TEX_LAND);
  shader->setInt("u_flat_diffuse2", TEX_LAND_2);
  shader->setInt("u_sand_diffuse", TEX_SHORE);
  shader->setInt("u_sand_diffuse2", TEX_SHORE_2);
  shader->setInt("u_diffuse_mix_map", TEX_DIFFUSE_MIX_MAP);
  shader->setInt("u_normal_map", TEX_TERRAIN_NORMAL);
  shader->setFloat("u_mapDimension", 1.0f / (float)WORLD_WIDTH);
  shader->setVec3("u_lightDir", glm::normalize(-LIGHT_DIR_TO));
  shader->setMat4("u_lightSpaceMatrix", LIGHT_SPACE_MATRIX);
  shader->setInt("u_shadowMap", TEX_DEPTH_MAP_SUN);
  shader->setFloat("U_UNDERWATER_TILE_YPOS", -UNDERWATER_TILE_YPOS);

  bindShaderUnit(shader, SHADER_UNDERWATER);
  shader->setInt("u_underwater_diffuse", TEX_UNDERWATER_DIFFUSE);
  shader->setInt("u_bottomRelief_diffuse", TEX_UNDERWATER_RELIEF);
  shader->setInt("u_normal_map", TEX_TERRAIN_NORMAL);
  shader->setVec3("u_lightDir", glm::normalize(-LIGHT_DIR_TO));
  shader->setFloat("u_mapDimension", 1.0f / (float)WORLD_WIDTH);

  bindShaderUnit(shader, SHADER_LAND);
  shader->setInt("u_flat_diffuse", TEX_LAND);
  shader->setInt("u_flat_diffuse2", TEX_LAND_2);
  shader->setInt("u_diffuse_mix_map", TEX_DIFFUSE_MIX_MAP);
  shader->setInt("u_normal_map", TEX_TERRAIN_NORMAL);
  shader->setFloat("u_mapDimension", 1.0f / (float)WORLD_WIDTH);
  shader->setVec3("u_lightDir", glm::normalize(-LIGHT_DIR_TO));
  shader->setMat4("u_lightSpaceMatrix", LIGHT_SPACE_MATRIX);
  shader->setInt("u_shadowMap", TEX_DEPTH_MAP_SUN);

  bindShaderUnit(shader, SHADER_WATER);
  shader->setVec3("u_lightDir", glm::normalize(-LIGHT_DIR_TO));
  shader->setInt("u_skybox", TEX_SKYBOX);
  shader->setInt("u_bottomRelief_diffuse", TEX_UNDERWATER_RELIEF);
  shader->setInt("u_normal_map", TEX_WATER_NORMAL);
  shader->setInt("u_specular_map", TEX_WATER_SPECULAR);
  shader->setFloat("u_mapDimension", 1.0f / WORLD_WIDTH);

  bindShaderUnit(shader, SHADER_SKYBOX);
  shader->setInt("u_skybox[1]", TEX_SKYBOX);
  shader->setInt("u_skybox[0]", TEX_SKYBOX_FAR);
  shader->setInt("u_skybox[2]", TEX_SKYBOX_SKY);

  bindShaderUnit(shader, SHADER_FONT);
  shader->setMat4("u_projection", fontProjection);
  shader->setInt("u_fontTexture", TEX_FONT);

  bindShaderUnit(shader, SHADER_MODELS);
  shader->setVec3("u_lightDir", glm::normalize(-LIGHT_DIR_TO));
  shader->setMat4("u_lightSpaceMatrix", LIGHT_SPACE_MATRIX);
  shader->setInt("u_shadowMap", TEX_DEPTH_MAP_SUN);
  shader->setInt("u_texture_diffuse1", TEX_MESH_DIFFUSE);
  shader->setInt("u_texture_specular1", TEX_MESH_SPECULAR);

  bindShaderUnit(shader, SHADER_MODELS_PHONG);
  shader->setVec3("u_lightDir", glm::normalize(-LIGHT_DIR_TO));
  shader->setMat4("u_lightSpaceMatrix", LIGHT_SPACE_MATRIX);
  shader->setInt("u_shadowMap", TEX_DEPTH_MAP_SUN);
  shader->setInt("u_texture_diffuse1", TEX_MESH_DIFFUSE);
  shader->setInt("u_texture_specular1", TEX_MESH_SPECULAR);

  bindShaderUnit(shader, SHADER_MS_TO_DEFAULT);
  shader->setInt("u_frameTexture", HDR_ENABLED ? TEX_FRAME_HDR : TEX_FRAME);
  shader->setFloat("u_exposure", 2.2f);

  bindShaderUnit(shader, SHADER_SHADOW_TERRAIN);
  shader->setMat4("u_lightSpaceMatrix", LIGHT_SPACE_MATRIX);

  bindShaderUnit(shader, SHADER_SHADOW_MODELS);
  shader->setMat4("u_lightSpaceMatrix", LIGHT_SPACE_MATRIX);
}

Shader &ShaderManager::get(SHADER type)
{
  return shaders[type];
}

void ShaderManager::updateHillsShaders(bool useFC, bool useShadows, glm::mat4 &projectionView, glm::vec3 &viewPosition, Frustum &viewFrustum, float maxHillHeight)
{
  Shader* shader = nullptr;
  if (useFC)
    {
      bindShaderUnit(shader, SHADER_HILLS_CULLING);
      shader->setVec4("u_frustumPlanes[0]", viewFrustum.getPlane(FRUSTUM_LEFT));
      shader->setVec4("u_frustumPlanes[1]", viewFrustum.getPlane(FRUSTUM_RIGHT));
      shader->setVec4("u_frustumPlanes[2]", viewFrustum.getPlane(FRUSTUM_BOTTOM));
      shader->setVec4("u_frustumPlanes[3]", viewFrustum.getPlane(FRUSTUM_TOP));
      shader->setVec4("u_frustumPlanes[4]", viewFrustum.getPlane(FRUSTUM_BACK));
    }
  bindShaderUnit(shader, SHADER_HILLS);
  shader->setMat4("u_projectionView", projectionView);
  shader->setVec3("u_viewPosition", viewPosition);
  shader->setBool("u_shadowEnable", useShadows);
  shader->setFloat("u_maxHillHeight", maxHillHeight);
}

void ShaderManager::updateShoreShader(glm::mat4 &projectionView, bool useShadows)
{
  Shader* shader = &shaders[SHADER_SHORE];
  shader->use();
  shader->setMat4("u_projectionView", projectionView);
  shader->setBool("u_shadowEnable", useShadows);
}

void ShaderManager::updateFlatShader(glm::mat4 &projectionView, bool useShadows)
{
  Shader* shader = &shaders[SHADER_LAND];
  shader->use();
  shader->setMat4("u_projectionView", projectionView);
  shader->setBool("u_shadowEnable", useShadows);
}

void ShaderManager::updateUnderwaterShader(glm::mat4 &projectionView)
{
  Shader* shader = &shaders[SHADER_UNDERWATER];
  shader->use();
  shader->setMat4("u_projectionView", projectionView);
}

void ShaderManager::updateBuildableShader(glm::mat4 &projectionView)
{
  Shader* shader = &shaders[SHADER_BUILDABLE];
  shader->use();
  shader->setMat4("u_projectionView", projectionView);
}

void ShaderManager::updateSelectedShader(glm::mat4 &projectionView, glm::mat4 &selectedModel)
{
  Shader* shader = &shaders[SHADER_SELECTED];
  shader->use();
  shader->setMat4("u_projectionView", projectionView);
  shader->setMat4("u_model", selectedModel);
}

void ShaderManager::updateWaterShaders(bool useFC, glm::mat4 &projectionView, glm::vec3 &viewPosition, Frustum &viewFrustum)
{
  Shader* shader = nullptr;
  if (useFC)
    {
      bindShaderUnit(shader, SHADER_WATER_CULLING);
      shader->setVec4("u_frustumPlanes[0]", viewFrustum.getPlane(FRUSTUM_LEFT));
      shader->setVec4("u_frustumPlanes[1]", viewFrustum.getPlane(FRUSTUM_RIGHT));
      shader->setVec4("u_frustumPlanes[2]", viewFrustum.getPlane(FRUSTUM_BOTTOM));
      shader->setVec4("u_frustumPlanes[3]", viewFrustum.getPlane(FRUSTUM_TOP));
      shader->setVec4("u_frustumPlanes[4]", viewFrustum.getPlane(FRUSTUM_BACK));
    }
  bindShaderUnit(shader, SHADER_WATER);
  shader->setMat4("u_projectionView", projectionView);
  shader->setVec3("u_viewPosition", viewPosition);
}

void ShaderManager::updateSkyShader(glm::mat4 &projectionView, glm::vec3& viewPosition, int backgroundIndex, bool isStatic)
{
  Shader* shader = &shaders[SHADER_SKYBOX];
  shader->use();
  shader->setMat4("u_projectionView", projectionView);
  shader->setVec3("u_viewPosition", viewPosition);
  shader->setInt("u_index", backgroundIndex);
  shader->setInt("u_static", isStatic);
}

void ShaderManager::updateModelShader(glm::mat4 &projectionView, glm::vec3 &viewPosition,
                                      bool shadowOnTrees,
                                      bool useShadows,
                                      bool useFlatBlending)
{
  Shader* shader = nullptr;
  bindShaderUnit(shader, SHADER_MODELS);
  shader->setMat4("u_projectionView", projectionView);
  shader->setVec3("u_viewPosition", viewPosition);
  shader->setBool("u_shadow", shadowOnTrees);
  shader->setBool("u_shadowEnable", useShadows);
  shader->setBool("u_useFlatBlending", useFlatBlending);

  bindShaderUnit(shader, SHADER_MODELS_PHONG);
  shader->setMat4("u_projectionView", projectionView);
  shader->setVec3("u_viewPosition", viewPosition);
  shader->setBool("u_shadow", shadowOnTrees);
  shader->setBool("u_shadowEnable", useShadows);
  shader->setBool("u_useFlatBlending", useFlatBlending);
}
