#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
  shaders.assign(
  {
    {SHADER_HILLS,              Shader("/shaders/hills.vs", "/shaders/hills.gs", "/shaders/hills.fs")},
    {SHADER_HILLS_NOFC,         Shader("/shaders/hills_noFC.vs", "/shaders/hills_noFC.fs")},
    {SHADER_SHORE,              Shader("/shaders/shore.vs", "/shaders/shore.fs")},
    {SHADER_UNDERWATER,         Shader("/shaders/underwater.vs", "/shaders/underwater.fs")},
    {SHADER_FLAT,               Shader("/shaders/flat.vs", "/shaders/flat.fs")},
    {SHADER_WATER,              Shader("/shaders/water.vs", "/shaders/water.gs", "/shaders/water.fs")},
    {SHADER_WATER_NOFC,         Shader("/shaders/water_noFC.vs", "/shaders/water_noFC.fs")},
    {SHADER_SKY,                Shader("/shaders/skybox.vs", "/shaders/skybox.fs")},
    {SHADER_MODELS,             Shader("/shaders/model.vs", "/shaders/model.fs")},
    {SHADER_FONT,               Shader("/shaders/font.vs", "/shaders/font.fs")},
    {SHADER_CS,                 Shader("/shaders/coordinateSystem.vs", "/shaders/coordinateSystem.gs", "/shaders/coordinateSystem.fs")},
    {SHADER_BUILDABLE,          Shader("/shaders/buildableTiles.vs", "/shaders/buildableTiles.fs")},
    {SHADER_SELECTED,           Shader("/shaders/selectedTile.vs", "/shaders/selectedTile.fs")},
    {SHADER_MS_TO_DEFAULT,      Shader("/shaders/MS_toDefault.vs", "/shaders/MS_toDefault.fs")},
    {SHADER_HDR,                Shader("/shaders/hdr.vs", "/shaders/hdr.fs")},
    {SHADER_SHADOW_FLAT,        Shader("/shaders/flat_shadow.vs", "/shaders/_depth.fs")},
    {SHADER_SHADOW_HILLS,       Shader("/shaders/hills_shadow.vs", "/shaders/_depth.fs")},
    {SHADER_SHADOW_MODELS,      Shader("/shaders/model_shadow.vs", "/shaders/_depth.fs")},
    {SHADER_SHADOW_SHORE,       Shader("/shaders/shore_shadow.vs", "/shaders/_depth.fs")},
    {SHADER_SHADOW_UNDERWATER,  Shader("/shaders/underwater_shadow.vs", "/shaders/_depth.fs")},
    {SHADER_SHADOW_WATER,       Shader("/shaders/water_shadow.vs", "/shaders/_depth.fs")},
        });
}

void ShaderManager::setupConstantUniforms()
{
  Shader* shader = &shaders[SHADER_HILLS].second;
  shader->use();
  shader->setVec3("u_lightDir", glm::normalize(-LIGHT_DIR_TO));
  shader->setInt("u_flat_diffuse", FLAT_x2);
  shader->setInt("u_flat_diffuse2", FLAT_2_x2);
  shader->setInt("u_hills_diffuse", HILL);
  shader->setInt("u_hills_diffuse2", HILL_2);
  shader->setInt("u_hills_specular", HILL_SPECULAR);
  shader->setInt("u_normal_map", NORMAL_MAP);
  shader->setInt("u_mapDimension", TILES_WIDTH);
  shader->setMat4("u_lightSpaceMatrix", LIGHT_SPACE_MATRIX);
  shader->setInt("u_shadowMap", DEPTH_MAP);

  shader = &shaders[SHADER_HILLS_NOFC].second;
  shader->use();
  shader->setVec3("u_lightDir", glm::normalize(-LIGHT_DIR_TO));
  shader->setInt("u_flat_diffuse", FLAT_x2);
  shader->setInt("u_flat_diffuse2", FLAT_2_x2);
  shader->setInt("u_hills_diffuse", HILL);
  shader->setInt("u_hills_diffuse2", HILL_2);
  shader->setInt("u_hills_specular", HILL_SPECULAR);
  shader->setInt("u_normal_map", NORMAL_MAP);
  shader->setInt("u_mapDimension", TILES_WIDTH);
  shader->setMat4("u_lightSpaceMatrix", LIGHT_SPACE_MATRIX);
  shader->setInt("u_shadowMap", DEPTH_MAP);

  shader = &shaders[SHADER_SHORE].second;
  shader->use();
  shader->setInt("u_flat_diffuse", FLAT);
  shader->setInt("u_flat_diffuse2", FLAT_2);
  shader->setInt("u_sand_diffuse", SHORE);
  shader->setInt("u_sand_diffuse2", SHORE_2);
  shader->setInt("u_normal_map", NORMAL_MAP);
  shader->setInt("u_mapDimension", TILES_WIDTH);
  shader->setVec3("u_lightDir", glm::normalize(-LIGHT_DIR_TO));
  shader->setMat4("u_lightSpaceMatrix", LIGHT_SPACE_MATRIX);
  shader->setInt("u_shadowMap", DEPTH_MAP);

  shader = &shaders[SHADER_UNDERWATER].second;
  shader->use();
  shader->setInt("u_underwater_diffuse", SHORE);
  shader->setInt("u_normal_map", NORMAL_MAP);
  shader->setInt("u_mapDimension", TILES_WIDTH);
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
  shader->setInt("u_shadowMap", DEPTH_MAP);

  shader = &shaders[SHADER_WATER].second;
  shader->use();
  shader->setVec3("u_lightDir", glm::normalize(-LIGHT_DIR_TO));
  shader->setInt("u_skybox", SKYBOX);
  shader->setInt("u_normal_map", NORMAL_MAP);
  shader->setInt("u_mapDimension", TILES_WIDTH * 4);

  shader = &shaders[SHADER_WATER_NOFC].second;
  shader->use();
  shader->setVec3("u_lightDir", glm::normalize(-LIGHT_DIR_TO));
  shader->setInt("u_skybox", SKYBOX);
  shader->setInt("u_normal_map", NORMAL_MAP);
  shader->setInt("u_mapDimension", TILES_WIDTH * 4);

  shader = &shaders[SHADER_SKY].second;
  shader->use();
  shader->setInt("u_skybox", SKYBOX);

  shader = &shaders[SHADER_MODELS].second;
  shader->use();
  shader->setVec3("u_lightDir", glm::normalize(-LIGHT_DIR_TO));
  shader->setMat4("u_lightSpaceMatrix", LIGHT_SPACE_MATRIX);
  shader->setInt("u_shadowMap", DEPTH_MAP);

  shader = &shaders[SHADER_HDR].second;
  shader->use();
  shader->setFloat("u_exposure", 2.2f);
  shader->setInt("u_frameTexture", FRAME_HDR_TEXTURE);

  shader = &shaders[SHADER_MS_TO_DEFAULT].second;
  shader->use();
  shader->setInt("u_frameTexture", FRAME_TEXTURE);

  //below we setup shadow shader uniforms
  shader = &shaders[SHADER_SHADOW_FLAT].second;
  shader->use();
  shader->setMat4("u_lightSpaceMatrix", LIGHT_SPACE_MATRIX);

  shader = &shaders[SHADER_SHADOW_HILLS].second;
  shader->use();
  shader->setMat4("u_lightSpaceMatrix", LIGHT_SPACE_MATRIX);

  shader = &shaders[SHADER_SHADOW_MODELS].second;
  shader->use();
  shader->setMat4("u_lightSpaceMatrix", LIGHT_SPACE_MATRIX);

  shader = &shaders[SHADER_SHADOW_SHORE].second;
  shader->use();
  shader->setMat4("u_lightSpaceMatrix", LIGHT_SPACE_MATRIX);

  shader = &shaders[SHADER_SHADOW_UNDERWATER].second;
  shader->use();
  shader->setMat4("u_lightSpaceMatrix", LIGHT_SPACE_MATRIX);

  shader = &shaders[SHADER_SHADOW_WATER].second;
  shader->use();
  shader->setMat4("u_lightSpaceMatrix", LIGHT_SPACE_MATRIX);
}

Shader &ShaderManager::get(SHADER_TYPE type)
{
  return shaders[type].second;
}

void ShaderManager::updateHillsShaders(bool enableFC, bool enableShadows, glm::mat4 &projectionView, glm::vec3 &viewPosition, Frustum &viewFrustum)
{
  Shader* shader;
  if (enableFC)
    {
      shader = &shaders[SHADER_HILLS].second;
      shader->use();
      shader->setMat4("u_projectionView", projectionView);
      shader->setVec3("u_viewPosition", viewPosition);
      shader->setVec4("u_frustumPlanes[0]", viewFrustum.getPlane(FRUSTUM_LEFT));
      shader->setVec4("u_frustumPlanes[1]", viewFrustum.getPlane(FRUSTUM_RIGHT));
      shader->setVec4("u_frustumPlanes[2]", viewFrustum.getPlane(FRUSTUM_BOTTOM));
      shader->setVec4("u_frustumPlanes[3]", viewFrustum.getPlane(FRUSTUM_TOP));
      shader->setBool("u_shadowEnable", enableShadows);
    }
  else
    {
      shader = &shaders[SHADER_HILLS_NOFC].second;
      shader->use();
      shader->setMat4("u_projectionView", projectionView);
      shader->setVec3("u_viewPosition", viewPosition);
      shader->setBool("u_shadowEnable", enableShadows);
    }
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
      shader = &shaders[SHADER_WATER].second;
      shader->use();
      shader->setMat4("u_projectionView", projectionView);
      shader->setVec3("u_viewPosition", viewPosition);
      shader->setVec4("u_frustumPlanes[0]", viewFrustum.getPlane(FRUSTUM_LEFT));
      shader->setVec4("u_frustumPlanes[1]", viewFrustum.getPlane(FRUSTUM_RIGHT));
      shader->setVec4("u_frustumPlanes[2]", viewFrustum.getPlane(FRUSTUM_BOTTOM));
      shader->setVec4("u_frustumPlanes[3]", viewFrustum.getPlane(FRUSTUM_TOP));
    }
  else
    {
      shader = &shaders[SHADER_WATER_NOFC].second;
      shader->use();
      shader->setMat4("u_projectionView", projectionView);
      shader->setVec3("u_viewPosition", viewPosition);
    }
}

void ShaderManager::updateSkyShader(glm::mat4 &view, glm::mat4 &projection)
{
  Shader* shader = &shaders[SHADER_SKY].second;
  shader->use();
  shader->setMat4("u_view", view);
  shader->setMat4("u_projection", projection);
}

void ShaderManager::updateModelShader(glm::mat4 &projectionView, glm::vec3 &viewPosition, bool shadowOnTrees, bool enableShadows)
{
  Shader* shader = &shaders[SHADER_MODELS].second;
  shader->use();
  shader->setMat4("u_projectionView", projectionView);
  shader->setVec3("u_viewPosition", viewPosition);
  shader->setBool("u_shadow", shadowOnTrees);
  shader->setBool("u_shadowEnable", enableShadows);
}

void ShaderManager::deleteShaders()
{
  for (unsigned int i = 0; i < shaders.size(); i++)
    shaders[i].second.cleanUp();
}
