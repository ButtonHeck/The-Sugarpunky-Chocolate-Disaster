#include "graphics/TextureManager.h"

TextureManager::TextureManager(TextureLoader& loader)
  :
    loader(loader)
{
  textures[TEX_LAND] = loader.loadTexture("land.jpg", TEX_LAND, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[TEX_LAND_2] = loader.loadTexture("land2.jpg", TEX_LAND_2, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[TEX_DIFFUSE_MIX_MAP] = loader.loadTexture("diffuseMixMap.jpg", TEX_DIFFUSE_MIX_MAP, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true, true, false, true);
  textures[TEX_TERRAIN_NORMAL] = loader.loadTexture("terrainNormal.jpg", TEX_TERRAIN_NORMAL, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true, true, false, true);
  textures[TEX_WATER_NORMAL] = loader.loadTexture("waterNormal.jpg", TEX_WATER_NORMAL, GL_REPEAT, GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST, true, true, false, true);
  textures[TEX_WATER_SPECULAR] = loader.loadTexture("waterSpecular.jpg", TEX_WATER_SPECULAR, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true, true, false, true);
  textures[TEX_WATER_DUDV] = loader.loadTexture("waterDUDV.png", TEX_WATER_DUDV, GL_MIRRORED_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true, true, false, true);
  textures[TEX_HILL] = loader.loadTexture("hill2.jpg", TEX_HILL, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[TEX_HILL_2] = loader.loadTexture("hill.jpg", TEX_HILL_2, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[TEX_HILL_SPECULAR] = loader.loadTexture("hill_specular.jpg", TEX_HILL_SPECULAR, GL_REPEAT, GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST, false, true, false, true);
  textures[TEX_SHORE] = loader.loadTexture("shore.jpg", TEX_SHORE, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[TEX_UNDERWATER_DIFFUSE] = loader.loadTexture("underwater.jpg", TEX_UNDERWATER_DIFFUSE, GL_REPEAT, GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST, false);
  textures[TEX_SHORE_2] = loader.loadTexture("shore2.jpg", TEX_SHORE_2, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[TEX_SKYBOX] = loader.loadCubemap("cubemap/", TEX_SKYBOX, false);
  textures[TEX_SKYBOX_NORMAL] = loader.loadCubemap("cubemapNormal/", TEX_SKYBOX_NORMAL, true);
  textures[TEX_SKYBOX_FAR] = loader.loadCubemap("cubemapFar/", TEX_SKYBOX_FAR, false);
  textures[TEX_SKYBOX_FAR_NORMAL] = loader.loadCubemap("cubemapFarNormal/", TEX_SKYBOX_FAR_NORMAL, true);
  textures[TEX_SKYBOX_SKY] = loader.loadCubemap("cubemapSky/", TEX_SKYBOX_SKY, false);
  textures[TEX_THE_SUN] = loader.loadTexture("theSun.png", TEX_THE_SUN, GL_REPEAT, GL_LINEAR, GL_LINEAR, true);
  textures[TEX_FRAME_MULTISAMPLED] = loader.createFrameMSTexture(MULTISAMPLES, TEX_FRAME_MULTISAMPLED);
  textures[TEX_FRAME] = loader.createFrameTexture(TEX_FRAME, false, false);
  textures[TEX_FRAME_HDR] = loader.createFrameTexture(TEX_FRAME_HDR, false, false);
  textures[TEX_FRAME_DEPTH] = loader.createFrameTexture(TEX_FRAME_DEPTH, true, false);
  textures[TEX_FRAME_VIGNETTE] = loader.loadTexture("vignetteHoneycomb.png", TEX_FRAME_VIGNETTE, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, false, true, false, true);
  textures[TEX_FRAME_WATER_REFLECTION] = loader.createFrameTextureSized(TEX_FRAME_WATER_REFLECTION, false, FRAME_WATER_REFLECTION_WIDTH, FRAME_WATER_REFLECTION_HEIGHT, true);
  textures[TEX_FRAME_WATER_REFRACTION] = loader.createFrameTextureSized(TEX_FRAME_WATER_REFRACTION, false, FRAME_WATER_REFRACTION_WIDTH, FRAME_WATER_REFRACTION_HEIGHT, true);
  textures[TEX_FRAME_WATER_REFRACTION_DEPTH] = loader.createFrameTextureSized(TEX_FRAME_WATER_REFRACTION_DEPTH, true, FRAME_WATER_REFRACTION_WIDTH, FRAME_WATER_REFRACTION_HEIGHT, true);
  textures[TEX_DEPTH_MAP_SUN] = loader.createDepthMapTexture(DEPTH_MAP_TEXTURE_WIDTH, DEPTH_MAP_TEXTURE_HEIGHT, TEX_DEPTH_MAP_SUN);
}

TextureManager::~TextureManager()
{
  for (unsigned int i = 0; i < textures.size(); i++)
    glDeleteTextures(1, &(textures[i]));
}

void TextureManager::createUnderwaterReliefTexture(const map2D_f &waterMap)
{
  textures[TEX_UNDERWATER_RELIEF] = loader.createUnderwaterReliefTexture(waterMap, TEX_UNDERWATER_RELIEF, GL_LINEAR, GL_LINEAR);
}

GLuint &TextureManager::get(int unit)
{
  return textures[unit];
}

TextureLoader &TextureManager::getLoader()
{
  return loader;
}
