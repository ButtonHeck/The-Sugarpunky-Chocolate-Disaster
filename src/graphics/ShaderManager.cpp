#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
  shaders.assign(
  {
    {SHADER_HILLS_FC,           Shader("/shaders/hillsFC.vs", "/shaders/hillsFC.gs", std::string("/shaders/_FC.fs"))},
    {SHADER_HILLS_NOFC,         Shader("/shaders/hills.vs", "/shaders/hills.fs", false)},
    {SHADER_SHORE,              Shader("/shaders/shore.vs", "/shaders/shore.fs", false)},
    {SHADER_UNDERWATER,         Shader("/shaders/underwater.vs", "/shaders/underwater.fs", false)},
    {SHADER_FLAT,               Shader("/shaders/flat.vs", "/shaders/flat.fs", false)},
    {SHADER_WATER_FC,           Shader("/shaders/waterFC.vs", "/shaders/waterFC.gs", std::string("/shaders/_FC.fs"))},
    {SHADER_WATER_NOFC,         Shader("/shaders/water.vs", "/shaders/water.fs", false)},
    {SHADER_SKY,                Shader("/shaders/skybox.vs", "/shaders/skybox.fs", false)},
    {SHADER_MODELS,             Shader("/shaders/model.vs", "/shaders/model.fs", false)},
    {SHADER_FONT,               Shader("/shaders/font.vs", "/shaders/font.fs", false)},
    {SHADER_CS,                 Shader("/shaders/coordinateSystem.vs", "/shaders/coordinateSystem.gs", std::string("/shaders/coordinateSystem.fs"))},
    {SHADER_BUILDABLE,          Shader("/shaders/buildableTiles.vs", "/shaders/buildableTiles.fs", false)},
    {SHADER_SELECTED,           Shader("/shaders/selectedTile.vs", "/shaders/selectedTile.fs", false)},
    {SHADER_MS_TO_DEFAULT,      Shader("/shaders/MS_toDefault.vs", "/shaders/MS_toDefault_hdr.fs", false)},
    {SHADER_SHADOW_TERRAIN,     Shader("/shaders/terrain_shadow.vs")},
    {SHADER_SHADOW_MODELS,      Shader("/shaders/model_shadow.vs")},
    {SHADER_SHADOW_TERRAIN_CAMERA,      Shader("/shaders/terrain_shadow.vs")},
    {SHADER_SHADOW_MODELS_CAMERA,       Shader("/shaders/model_shadow.vs")},
        });
}

void ShaderManager::setupConstantUniforms(int scr_width, int scr_height)
{
  Shader* shader = &shaders[SHADER_HILLS_NOFC].second;
  shader->use();
  shader->setVec3("u_lightDir", glm::normalize(-LIGHT_DIR_TO));
  shader->setInt("u_flat_diffuse", FLAT_x2);
  shader->setInt("u_flat_diffuse2", FLAT_2_x2);
  shader->setInt("u_hills_diffuse", HILL);
  shader->setInt("u_hills_diffuse2", HILL_2);
  shader->setInt("u_hills_specular", HILL_SPECULAR);
  shader->setInt("u_normal_map", NORMAL_MAP);
  shader->setFloat("u_mapDimension", 1.0f / (float)TILES_WIDTH);
  shader->setMat4("u_lightSpaceMatrix", LIGHT_SPACE_MATRIX);
  shader->setInt("u_shadowMap", DEPTH_MAP_SUN);
  shader->setInt("u_occlusionMap", DEPTH_MAP_CAMERA);
  shader->setFloat("U_NEAR", NEAR_PLANE);
  shader->setFloat("U_FAR", FAR_PLANE);
  shader->setFloat("U_SCR_WIDTH", scr_width);
  shader->setFloat("U_SCR_HEIGHT", scr_height);

  shader = &shaders[SHADER_SHORE].second;
  shader->use();
  shader->setInt("u_flat_diffuse", FLAT);
  shader->setInt("u_flat_diffuse2", FLAT_2);
  shader->setInt("u_sand_diffuse", SHORE);
  shader->setInt("u_sand_diffuse2", SHORE_2);
  shader->setInt("u_normal_map", NORMAL_MAP);
  shader->setFloat("u_mapDimension", 1.0f / (float)TILES_WIDTH);
  shader->setVec3("u_lightDir", glm::normalize(-LIGHT_DIR_TO));
  shader->setMat4("u_lightSpaceMatrix", LIGHT_SPACE_MATRIX);
  shader->setInt("u_shadowMap", DEPTH_MAP_SUN);

  shader = &shaders[SHADER_UNDERWATER].second;
  shader->use();
  shader->setInt("u_underwater_diffuse", SHORE);
  shader->setInt("u_normal_map", NORMAL_MAP);
  shader->setFloat("u_mapDimension", 1.0f / (float)TILES_WIDTH);
  shader->setVec3("u_lightDir", glm::normalize(-LIGHT_DIR_TO));
  shader->setInt("u_bottomRelief_diffuse", UNDERWATER_RELIEF);

  shader = &shaders[SHADER_FLAT].second;
  shader->use();
  shader->setInt("u_flat_diffuse", FLAT);
  shader->setInt("u_flat_diffuse2", FLAT_2);
  shader->setInt("u_normal_map", NORMAL_MAP);
  shader->setFloat("u_mapDimension", 1.0f / (float)TILES_WIDTH);
  shader->setVec3("u_lightDir", glm::normalize(-LIGHT_DIR_TO));
  shader->setMat4("u_lightSpaceMatrix", LIGHT_SPACE_MATRIX);
  shader->setInt("u_shadowMap", DEPTH_MAP_SUN);

  shader = &shaders[SHADER_WATER_NOFC].second;
  shader->use();
  shader->setVec3("u_lightDir", glm::normalize(-LIGHT_DIR_TO));
  shader->setInt("u_skybox", SKYBOX);
  shader->setInt("u_normal_map", NORMAL_MAP);
  shader->setFloat("u_mapDimension", 1.0f / (TILES_WIDTH * 4));

  shader = &shaders[SHADER_SKY].second;
  shader->use();
  shader->setInt("u_skybox", SKYBOX);

  shader = &shaders[SHADER_MODELS].second;
  shader->use();
  shader->setVec3("u_lightDir", glm::normalize(-LIGHT_DIR_TO));
  shader->setMat4("u_lightSpaceMatrix", LIGHT_SPACE_MATRIX);
  shader->setInt("u_shadowMap", DEPTH_MAP_SUN);
  shader->setInt("u_occlusionMap", DEPTH_MAP_CAMERA);
  shader->setFloat("U_NEAR", NEAR_PLANE);
  shader->setFloat("U_FAR", FAR_PLANE);
  shader->setFloat("U_SCR_WIDTH", scr_width);
  shader->setFloat("U_SCR_HEIGHT", scr_height);

  shader = &shaders[SHADER_MS_TO_DEFAULT].second;
  shader->use();
  shader->setInt("u_frameTexture", HDR_ENABLED ? FRAME_HDR_TEXTURE : FRAME_TEXTURE);
  shader->setFloat("u_exposure", 2.2f);

  //below we setup shadow shader uniforms
  shader = &shaders[SHADER_SHADOW_TERRAIN].second;
  shader->use();
  shader->setMat4("u_lightSpaceMatrix", LIGHT_SPACE_MATRIX);

  shader = &shaders[SHADER_SHADOW_MODELS].second;
  shader->use();
  shader->setMat4("u_lightSpaceMatrix", LIGHT_SPACE_MATRIX);
}

Shader &ShaderManager::get(SHADER_TYPE type)
{
  return shaders[type].second;
}

void ShaderManager::updateHillsShaders(bool useFC, bool useOC, bool enableShadows, glm::mat4 &projectionView, glm::vec3 &viewPosition, Frustum &viewFrustum)
{
  Shader* shader;
  if (useFC)
    {
      shader = &shaders[SHADER_HILLS_FC].second;
      shader->use();
      shader->setVec4("u_frustumPlanes[0]", viewFrustum.getPlane(FRUSTUM_LEFT));
      shader->setVec4("u_frustumPlanes[1]", viewFrustum.getPlane(FRUSTUM_RIGHT));
      shader->setVec4("u_frustumPlanes[2]", viewFrustum.getPlane(FRUSTUM_BOTTOM));
      shader->setVec4("u_frustumPlanes[3]", viewFrustum.getPlane(FRUSTUM_TOP));
      shader->setVec4("u_frustumPlanes[4]", viewFrustum.getPlane(FRUSTUM_BACK));
    }
  shader = &shaders[SHADER_HILLS_NOFC].second;
  shader->use();
  shader->setMat4("u_projectionView", projectionView);
  shader->setVec3("u_viewPosition", viewPosition);
  shader->setBool("u_shadowEnable", enableShadows);
  shader->setBool("u_occlusion", useOC);
}

void ShaderManager::updateShoreShader(glm::mat4 &projectionView, bool enableShadows)
{
  Shader* shader = &shaders[SHADER_SHORE].second;
  shader->use();
  shader->setMat4("u_projectionView", projectionView);
  shader->setBool("u_shadowEnable", enableShadows);
}

void ShaderManager::updateFlatShader(glm::mat4 &projectionView, bool enableShadows)
{
  Shader* shader = &shaders[SHADER_FLAT].second;
  shader->use();
  shader->setMat4("u_projectionView", projectionView);
  shader->setBool("u_shadowEnable", enableShadows);
}

void ShaderManager::updateUnderwaterShader(glm::mat4 &projectionView)
{
  Shader* shader = &shaders[SHADER_UNDERWATER].second;
  shader->use();
  shader->setMat4("u_projectionView", projectionView);
}

void ShaderManager::updateBuildableShader(glm::mat4 &projectionView)
{
  Shader* shader = &shaders[SHADER_BUILDABLE].second;
  shader->use();
  shader->setMat4("u_projectionView", projectionView);
}

void ShaderManager::updateSelectedShader(glm::mat4 &projectionView, glm::mat4 &selectedModel)
{
  Shader* shader = &shaders[SHADER_SELECTED].second;
  shader->use();
  shader->setMat4("u_projectionView", projectionView);
  shader->setMat4("u_model", selectedModel);
}

void ShaderManager::updateWaterShaders(bool enableFC, glm::mat4 &projectionView, glm::vec3 &viewPosition, Frustum &viewFrustum)
{
  Shader* shader;
  if (enableFC)
    {
      shader = &shaders[SHADER_WATER_FC].second;
      shader->use();
      shader->setVec4("u_frustumPlanes[0]", viewFrustum.getPlane(FRUSTUM_LEFT));
      shader->setVec4("u_frustumPlanes[1]", viewFrustum.getPlane(FRUSTUM_RIGHT));
      shader->setVec4("u_frustumPlanes[2]", viewFrustum.getPlane(FRUSTUM_BOTTOM));
      shader->setVec4("u_frustumPlanes[3]", viewFrustum.getPlane(FRUSTUM_TOP));
      shader->setVec4("u_frustumPlanes[4]", viewFrustum.getPlane(FRUSTUM_BACK));
    }
  shader = &shaders[SHADER_WATER_NOFC].second;
  shader->use();
  shader->setMat4("u_projectionView", projectionView);
  shader->setVec3("u_viewPosition", viewPosition);
}

void ShaderManager::updateSkyShader(glm::mat4 &projectionView)
{
  Shader* shader = &shaders[SHADER_SKY].second;
  shader->use();
  shader->setMat4("u_projectionView", projectionView);
}

void ShaderManager::updateModelShader(glm::mat4 &projectionView, glm::vec3 &viewPosition, bool shadowOnTrees, bool enableShadows, bool useOC)
{
  Shader* shader = &shaders[SHADER_MODELS].second;
  shader->use();
  shader->setMat4("u_projectionView", projectionView);
  shader->setVec3("u_viewPosition", viewPosition);
  shader->setBool("u_shadow", shadowOnTrees);
  shader->setBool("u_shadowEnable", enableShadows);
  shader->setBool("u_occlusion", useOC);
}

void ShaderManager::deleteShaders()
{
  glUseProgram(0);
  for (unsigned int i = 0; i < shaders.size(); i++)
    shaders[i].second.cleanUp();
}
