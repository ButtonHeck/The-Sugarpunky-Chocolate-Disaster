#include "graphics/ShaderManager.h"

ShaderManager::ShaderManager()
{
  shaders[SHADER_HILLS_CULLING] = Shader("hillsFC/hillsFC.vs",
                                         "hillsFC/hillsFC.gs",
                                         "_FC.fs");
  shaders[SHADER_HILLS] = Shader("hills/hills.vs",
                                 "hills/hills.fs",
                                 {{GL_FRAGMENT_SHADER, "shadowSampling.ifs"},
                                 {GL_FRAGMENT_SHADER, "desaturationFunc.ifs"},
                                 {GL_FRAGMENT_SHADER, "shadingVariables.ifs"}});
  shaders[SHADER_HILLS_NORMALS] = Shader("normals/hills_normals.vs",
                                         "normals/_normals.gs",
                                         "normals/_normals.fs");
  shaders[SHADER_SHORE] = Shader("shore/shore.vs",
                                 "shore/shore.fs",
                                 {{GL_FRAGMENT_SHADER, "shadowSampling.ifs"},
                                 {GL_FRAGMENT_SHADER, "desaturationFunc.ifs"},
                                 {GL_FRAGMENT_SHADER, "shadingVariables.ifs"}});
  shaders[SHADER_SHORE_NORMALS] = Shader("normals/shore_normals.vs",
                                         "normals/_normals.gs",
                                         "normals/_normals.fs");
  shaders[SHADER_UNDERWATER] = Shader("underwater/underwater.vs",
                                      "underwater/underwater.fs",
                                      {{GL_FRAGMENT_SHADER, "desaturationFunc.ifs"},
                                      {GL_FRAGMENT_SHADER, "shadingVariables.ifs"}});
  shaders[SHADER_LAND] = Shader("land/land.vs",
                                "land/land.fs",
                                {{GL_FRAGMENT_SHADER, "shadowSampling.ifs"},
                                {GL_FRAGMENT_SHADER, "desaturationFunc.ifs"},
                                {GL_FRAGMENT_SHADER, "shadingVariables.ifs"}});
  shaders[SHADER_WATER_CULLING] = Shader("waterFC/waterFC.vs",
                                         "waterFC/waterFC.gs",
                                         "_FC.fs");
  shaders[SHADER_WATER] = Shader("water/water.vs",
                                 "water/water.fs",
                                 {{GL_FRAGMENT_SHADER, "shadowSampling.ifs"},
                                 {GL_FRAGMENT_SHADER, "desaturationFunc.ifs"},
                                 {GL_FRAGMENT_SHADER, "shadingVariables.ifs"}});
  shaders[SHADER_WATER_NORMALS] = Shader("normals/water_normals.vs",
                                         "normals/_normals.gs",
                                         "normals/_normals.fs");
  shaders[SHADER_SKYBOX] = Shader("skybox/skybox.vs",
                                  "skybox/skybox.fs");
  shaders[SHADER_SUN] = Shader("theSun/theSun.vs",
                               "theSun/theSun.fs");
  shaders[SHADER_MODELS] = Shader("modelGouraud/model.vs",
                                  "modelGouraud/model.fs",
                                  {{GL_VERTEX_SHADER, "modelGrassAnimationAndBlending.ivs"},
                                  {GL_FRAGMENT_SHADER, "shadowSampling.ifs"},
                                  {GL_FRAGMENT_SHADER, "desaturationFunc.ifs"},
                                  {GL_FRAGMENT_SHADER, "shadingVariables.ifs"}});
  shaders[SHADER_MODELS_PHONG] = Shader("modelPhong/modelPhong.vs",
                                        "modelPhong/modelPhong.fs",
                                        {{GL_VERTEX_SHADER, "modelGrassAnimationAndBlending.ivs"},
                                        {GL_FRAGMENT_SHADER, "shadowSampling.ifs"},
                                        {GL_FRAGMENT_SHADER, "desaturationFunc.ifs"},
                                        {GL_FRAGMENT_SHADER, "shadingVariables.ifs"}});
  shaders[SHADER_FONT] = Shader("font/font.vs",
                                "font/font.fs");
  shaders[SHADER_COORDINATE_SYSTEM] = Shader("coordinateSystem/coordinateSystem.vs",
                                             "coordinateSystem/coordinateSystem.gs",
                                             "coordinateSystem/coordinateSystem.fs");
  shaders[SHADER_BUILDABLE] = Shader("buildable/buildableTiles.vs",
                                     "buildable/buildableTiles.fs");
  shaders[SHADER_SELECTED] = Shader("selected/selectedTile.vs",
                                    "selected/selectedTile.fs");
  shaders[SHADER_MS_TO_DEFAULT] = Shader("screen/MS_toDefault.vs",
                                         "screen/MS_toDefault_hdr.fs");
  shaders[SHADER_SHADOW_TERRAIN] = Shader("shadow/terrain_shadow.vs",
                                          "shadow/shadow.gs",
                                          "_FC.fs");
  shaders[SHADER_SHADOW_MODELS] = Shader("shadow/model_shadow.vs",
                                         "shadow/shadow.gs",
                                         "_FC.fs");
  shaders[SHADER_FRUSTUM] = Shader("frustum/frustum.vs",
                                   "frustum/frustum.fs");
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
  BENCHMARK("Shader Manager: setup", false);

  Shader* shader = nullptr;
  bindShaderUnit(shader, SHADER_HILLS);
  shader->setInt("u_land_diffuse[0]", TEX_LAND);
  shader->setInt("u_land_diffuse[1]", TEX_LAND_2);
  shader->setInt("u_hills_diffuse[0]", TEX_HILL);
  shader->setInt("u_hills_diffuse[1]", TEX_HILL_2);
  shader->setInt("u_hills_specular", TEX_HILL_SPECULAR);
  shader->setInt("u_diffuse_mix_map", TEX_DIFFUSE_MIX_MAP);
  shader->setInt("u_normal_map", TEX_TERRAIN_NORMAL);
  shader->setFloat("u_mapDimension", 1.0f / (float)WORLD_WIDTH);
  shader->setInt("u_shadowMap", TEX_DEPTH_MAP_SUN);
  shader->setFloat("u_bias", 6.0f / DEPTH_MAP_TEXTURE_WIDTH);
  shader->setFloat("u_ambientDay", 0.08f);
  shader->setFloat("u_ambientNight", 0.03f);

  bindShaderUnit(shader, SHADER_SHORE);
  shader->setInt("u_land_diffuse[0]", TEX_LAND);
  shader->setInt("u_land_diffuse[1]", TEX_LAND_2);
  shader->setInt("u_sand_diffuse[0]", TEX_SHORE);
  shader->setInt("u_sand_diffuse[1]", TEX_SHORE_2);
  shader->setInt("u_diffuse_mix_map", TEX_DIFFUSE_MIX_MAP);
  shader->setInt("u_normal_map", TEX_TERRAIN_NORMAL);
  shader->setFloat("u_mapDimension", 1.0f / (float)WORLD_WIDTH);
  shader->setInt("u_shadowMap", TEX_DEPTH_MAP_SUN);
  shader->setFloat("U_UNDERWATER_TILE_YPOS", -UNDERWATER_TILE_YPOS);
  shader->setFloat("u_bias", 1.0f / DEPTH_MAP_TEXTURE_WIDTH);
  shader->setFloat("u_ambientDay", 0.08f);
  shader->setFloat("u_ambientNight", 0.03f);

  bindShaderUnit(shader, SHADER_UNDERWATER);
  shader->setInt("u_underwater_diffuse", TEX_UNDERWATER_DIFFUSE);
  shader->setInt("u_bottomRelief_diffuse", TEX_UNDERWATER_RELIEF);
  shader->setInt("u_normal_map", TEX_TERRAIN_NORMAL);
  shader->setFloat("u_mapDimension", 1.0f / (float)WORLD_WIDTH);
  shader->setFloat("u_ambientDay", 0.08f);
  shader->setFloat("u_ambientNight", 0.03f);

  bindShaderUnit(shader, SHADER_LAND);
  shader->setInt("u_land_diffuse[0]", TEX_LAND);
  shader->setInt("u_land_diffuse[1]", TEX_LAND_2);
  shader->setInt("u_diffuse_mix_map", TEX_DIFFUSE_MIX_MAP);
  shader->setInt("u_normal_map", TEX_TERRAIN_NORMAL);
  shader->setFloat("u_mapDimension", 1.0f / (float)WORLD_WIDTH);
  shader->setInt("u_shadowMap", TEX_DEPTH_MAP_SUN);
  shader->setFloat("u_bias", 6.0f / DEPTH_MAP_TEXTURE_WIDTH);
  shader->setFloat("u_ambientDay", 0.08f);
  shader->setFloat("u_ambientNight", 0.03f);

  bindShaderUnit(shader, SHADER_WATER);
  shader->setInt("u_skybox", TEX_SKYBOX_SKY);
  shader->setInt("u_normal_map", TEX_WATER_NORMAL);
  shader->setInt("u_specular_map", TEX_WATER_SPECULAR);
  shader->setInt("u_shadowMap", TEX_DEPTH_MAP_SUN);
  shader->setFloat("u_bias", 4.0f / DEPTH_MAP_TEXTURE_WIDTH);
  shader->setFloat("u_ambientDay", 0.08f);
  shader->setFloat("u_ambientNight", 0.03f);

  bindShaderUnit(shader, SHADER_SKYBOX);
  shader->setInt("u_skybox[1]", TEX_SKYBOX);
  shader->setInt("u_skybox[0]", TEX_SKYBOX_FAR);
  shader->setInt("u_skybox[2]", TEX_SKYBOX_SKY);

  bindShaderUnit(shader, SHADER_SUN);
  shader->setInt("u_texture", TEX_THE_SUN);

  bindShaderUnit(shader, SHADER_FONT);
  shader->setMat4("u_projection", fontProjection);
  shader->setInt("u_fontTexture", TEX_FONT);

  bindShaderUnit(shader, SHADER_MODELS);
  shader->setInt("u_shadowMap", TEX_DEPTH_MAP_SUN);
  shader->setInt("u_texture_diffuse1", TEX_MESH_DIFFUSE);
  shader->setInt("u_texture_specular1", TEX_MESH_SPECULAR);
  shader->setFloat("u_bias", 3.0f / DEPTH_MAP_TEXTURE_WIDTH);
  shader->setFloat("u_ambientDay", 0.12f);
  shader->setFloat("u_ambientNight", 0.03f);

  bindShaderUnit(shader, SHADER_MODELS_PHONG);
  shader->setInt("u_shadowMap", TEX_DEPTH_MAP_SUN);
  shader->setInt("u_texture_diffuse1", TEX_MESH_DIFFUSE);
  shader->setInt("u_texture_specular1", TEX_MESH_SPECULAR);
  shader->setFloat("u_bias", 3.0f / DEPTH_MAP_TEXTURE_WIDTH);
  shader->setFloat("u_ambientDay", 0.12f);
  shader->setFloat("u_ambientNight", 0.03f);

  bindShaderUnit(shader, SHADER_MS_TO_DEFAULT);
  shader->setInt("u_frameTexture", HDR_ENABLED ? TEX_FRAME_HDR : TEX_FRAME);
  shader->setFloat("u_exposure", 2.2f);

  bindShaderUnit(shader, SHADER_WATER_NORMALS);
  shader->setInt("u_normal_map", TEX_TERRAIN_NORMAL);

  bindShaderUnit(shader, SHADER_HILLS_NORMALS);
  shader->setInt("u_normal_map", TEX_TERRAIN_NORMAL);

  bindShaderUnit(shader, SHADER_SHORE_NORMALS);
  shader->setInt("u_normal_map", TEX_TERRAIN_NORMAL);
}

Shader &ShaderManager::get(SHADER type)
{
  return shaders[type];
}
