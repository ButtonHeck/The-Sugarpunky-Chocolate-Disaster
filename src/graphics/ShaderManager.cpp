#include "graphics/ShaderManager.h"

ShaderManager::ShaderManager()
{
  shaders[SHADER_HILLS_CULLING] = Shader({GL_VERTEX_SHADER, "hillsFC/hillsFC.vs"},
                                         {GL_GEOMETRY_SHADER, "hillsFC/hillsFC.gs"});
  shaders[SHADER_HILLS] = Shader({GL_VERTEX_SHADER, "hills/hills.vs"},
                                 {GL_FRAGMENT_SHADER, "hills/hills.fs"},
                                 {{GL_FRAGMENT_SHADER, "shadowSampling.ifs"},
                                 {GL_FRAGMENT_SHADER, "desaturationFunc.ifs"},
                                 {GL_FRAGMENT_SHADER, "shadingVariables.ifs"}});
  shaders[SHADER_HILLS_NORMALS] = Shader({GL_VERTEX_SHADER, "normals/hills_normals.vs"},
                                         {GL_GEOMETRY_SHADER, "normals/_normals.gs"},
                                         {GL_FRAGMENT_SHADER, "normals/_normals.fs"});
  shaders[SHADER_SHORE] = Shader({GL_VERTEX_SHADER, "shore/shore.vs"},
                                 {GL_FRAGMENT_SHADER, "shore/shore.fs"},
                                 {{GL_FRAGMENT_SHADER, "shadowSampling.ifs"},
                                 {GL_FRAGMENT_SHADER, "desaturationFunc.ifs"},
                                 {GL_FRAGMENT_SHADER, "shadingVariables.ifs"}});
  shaders[SHADER_SHORE_NORMALS] = Shader({GL_VERTEX_SHADER, "normals/shore_normals.vs"},
                                         {GL_GEOMETRY_SHADER, "normals/_normals.gs"},
                                         {GL_FRAGMENT_SHADER, "normals/_normals.fs"});
  shaders[SHADER_UNDERWATER] = Shader({GL_VERTEX_SHADER, "underwater/underwater.vs"},
                                      {GL_FRAGMENT_SHADER, "underwater/underwater.fs"},
                                      {{GL_FRAGMENT_SHADER, "desaturationFunc.ifs"},
                                      {GL_FRAGMENT_SHADER, "shadingVariables.ifs"}});
  shaders[SHADER_LAND] = Shader({GL_VERTEX_SHADER, "land/land.vs"},
                                {GL_FRAGMENT_SHADER, "land/land.fs"},
                                {{GL_FRAGMENT_SHADER, "shadowSampling.ifs"},
                                {GL_FRAGMENT_SHADER, "desaturationFunc.ifs"},
                                {GL_FRAGMENT_SHADER, "shadingVariables.ifs"}});
  shaders[SHADER_WATER_CULLING] = Shader({GL_VERTEX_SHADER, "waterFC/waterFC.vs"},
                                         {GL_GEOMETRY_SHADER, "waterFC/waterFC.gs"});
  shaders[SHADER_WATER] = Shader({GL_VERTEX_SHADER, "water/water.vs"},
                                 {GL_FRAGMENT_SHADER, "water/water.fs"},
                                 {{GL_FRAGMENT_SHADER, "shadowSampling.ifs"},
                                 {GL_FRAGMENT_SHADER, "desaturationFunc.ifs"},
                                 {GL_FRAGMENT_SHADER, "shadingVariables.ifs"}});
  shaders[SHADER_WATER_NORMALS] = Shader({GL_VERTEX_SHADER, "normals/water_normals.vs"},
                                         {GL_GEOMETRY_SHADER, "normals/_normals.gs"},
                                         {GL_FRAGMENT_SHADER, "normals/_normals.fs"});
  shaders[SHADER_SKYBOX] = Shader({GL_VERTEX_SHADER, "skybox/skybox.vs"},
                                  {GL_FRAGMENT_SHADER, "skybox/skybox.fs"},
                                  {{GL_FRAGMENT_SHADER, "shadingVariables.ifs"},
                                  {GL_FRAGMENT_SHADER, "desaturationFunc.ifs"}});
  shaders[SHADER_SUN] = Shader({GL_VERTEX_SHADER, "theSun/theSun.vs"},
                               {GL_FRAGMENT_SHADER, "theSun/theSun.fs"});
  shaders[SHADER_MODELS_GOURAUD] = Shader({GL_VERTEX_SHADER, "modelGouraud/model.vs"},
                                  {GL_FRAGMENT_SHADER, "modelGouraud/model.fs"},
                                  {{GL_VERTEX_SHADER, "modelGrassAnimationAndBlending.ivs"},
                                  {GL_FRAGMENT_SHADER, "shadowSampling.ifs"},
                                  {GL_FRAGMENT_SHADER, "desaturationFunc.ifs"},
                                  {GL_FRAGMENT_SHADER, "shadingVariables.ifs"}});
  shaders[SHADER_MODELS_PHONG] = Shader({GL_VERTEX_SHADER, "modelPhong/modelPhong.vs"},
                                        {GL_FRAGMENT_SHADER, "modelPhong/modelPhong.fs"},
                                        {{GL_VERTEX_SHADER, "modelGrassAnimationAndBlending.ivs"},
                                        {GL_FRAGMENT_SHADER, "shadowSampling.ifs"},
                                        {GL_FRAGMENT_SHADER, "desaturationFunc.ifs"},
                                        {GL_FRAGMENT_SHADER, "shadingVariables.ifs"}});
  shaders[SHADER_FONT] = Shader({GL_VERTEX_SHADER, "font/font.vs"},
                                {GL_FRAGMENT_SHADER, "font/font.fs"});
  shaders[SHADER_COORDINATE_SYSTEM] = Shader({GL_VERTEX_SHADER, "coordinateSystem/coordinateSystem.vs"},
                                             {GL_GEOMETRY_SHADER, "coordinateSystem/coordinateSystem.gs"},
                                             {GL_FRAGMENT_SHADER, "coordinateSystem/coordinateSystem.fs"});
  shaders[SHADER_BUILDABLE] = Shader({GL_VERTEX_SHADER, "buildable/buildableTiles.vs"},
                                     {GL_FRAGMENT_SHADER, "buildable/buildableTiles.fs"});
  shaders[SHADER_SELECTED] = Shader({GL_VERTEX_SHADER, "selected/selectedTile.vs"},
                                    {GL_FRAGMENT_SHADER, "selected/selectedTile.fs"});
  shaders[SHADER_MS_TO_DEFAULT] = Shader({GL_VERTEX_SHADER, "screen/MS_toDefault.vs"},
                                         {GL_FRAGMENT_SHADER, "screen/MS_toDefault_hdr.fs"});
  shaders[SHADER_SHADOW_TERRAIN] = Shader({GL_VERTEX_SHADER, "shadow/terrain_shadow.vs"},
                                          {GL_GEOMETRY_SHADER, "shadow/shadow.gs"});
  shaders[SHADER_SHADOW_MODELS] = Shader({GL_VERTEX_SHADER, "shadow/model_shadow.vs"},
                                         {GL_GEOMETRY_SHADER, "shadow/shadow.gs"});
  shaders[SHADER_FRUSTUM] = Shader({GL_VERTEX_SHADER, "frustum/frustum.vs"},
                                   {GL_FRAGMENT_SHADER, "frustum/frustum.fs"});
  shaders[SHADER_LENS_FLARE] = Shader({GL_VERTEX_SHADER, "lensFlare/lensFlare.vs"},
                                      {GL_FRAGMENT_SHADER, "lensFlare/lensFlare.fs"});
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

void ShaderManager::setupConstantUniforms(glm::mat4 fontProjection, float aspectRatio)
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
  shader->setFloat("u_bias", 5.0f / DEPTH_MAP_TEXTURE_WIDTH);
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
  shader->setFloat("u_bias", 5.0f / DEPTH_MAP_TEXTURE_WIDTH);
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
  shader->setInt("u_skyboxNormals[1]", TEX_SKYBOX_NORMAL);
  shader->setInt("u_skybox[0]", TEX_SKYBOX_FAR);
  shader->setInt("u_skyboxNormals[0]", TEX_SKYBOX_FAR_NORMAL);
  shader->setInt("u_skybox[2]", TEX_SKYBOX_SKY);
  shader->setFloat("u_ambientDay", 0.08f);
  shader->setFloat("u_ambientNight", 0.03f);

  bindShaderUnit(shader, SHADER_SUN);
  shader->setInt("u_texture", TEX_THE_SUN);

  bindShaderUnit(shader, SHADER_FONT);
  shader->setMat4("u_projection", fontProjection);
  shader->setInt("u_fontTexture", TEX_FONT);

  bindShaderUnit(shader, SHADER_MODELS_GOURAUD);
  shader->setInt("u_shadowMap", TEX_DEPTH_MAP_SUN);
  shader->setFloat("u_bias", 8.0f / DEPTH_MAP_TEXTURE_WIDTH);
  shader->setFloat("u_ambientDay", 0.16f);
  shader->setFloat("u_ambientNight", 0.03f);

  bindShaderUnit(shader, SHADER_MODELS_PHONG);
  shader->setInt("u_shadowMap", TEX_DEPTH_MAP_SUN);
  shader->setFloat("u_bias", 8.0f / DEPTH_MAP_TEXTURE_WIDTH);
  shader->setFloat("u_ambientDay", 0.16f);
  shader->setFloat("u_ambientNight", 0.03f);

  bindShaderUnit(shader, SHADER_MS_TO_DEFAULT);
  shader->setInt("u_frameTexture", HDR_ENABLED ? TEX_FRAME_HDR : TEX_FRAME);
  shader->setInt("u_frameDepthTexture", TEX_FRAME_DEPTH);
  shader->setInt("u_vignetteTexture", TEX_FRAME_VIGNETTE);
  shader->setFloat("u_exposure", 2.2f);
  shader->setFloat("u_near", NEAR_PLANE);
  shader->setFloat("u_far", FAR_PLANE);
  shader->setFloat("u_aspectRatio", aspectRatio);

  bindShaderUnit(shader, SHADER_WATER_NORMALS);
  shader->setInt("u_normal_map", TEX_TERRAIN_NORMAL);

  bindShaderUnit(shader, SHADER_HILLS_NORMALS);
  shader->setInt("u_normal_map", TEX_TERRAIN_NORMAL);

  bindShaderUnit(shader, SHADER_SHORE_NORMALS);
  shader->setInt("u_normal_map", TEX_TERRAIN_NORMAL);

  bindShaderUnit(shader, SHADER_LENS_FLARE);
  shader->setInt("u_flares[0]", TEX_LENS_FLARE1);
  shader->setInt("u_flares[1]", TEX_LENS_FLARE2);
  shader->setInt("u_flares[2]", TEX_LENS_FLARE3);
  shader->setInt("u_flares[3]", TEX_LENS_FLARE4);
  shader->setInt("u_flares[4]", TEX_LENS_FLARE5);
  shader->setInt("u_flares[5]", TEX_LENS_FLARE6);
  shader->setInt("u_flares[6]", TEX_LENS_FLARE7);
}

Shader &ShaderManager::get(SHADER type)
{
  return shaders[type];
}
