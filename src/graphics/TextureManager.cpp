#include "graphics/TextureManager.h"

TextureManager::TextureManager(TextureLoader& loader)
  :
    loader(loader)
{
  textures[TEX_FLAT] = loader.loadTexture("flat.jpg", TEX_FLAT, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[TEX_FLAT_2] = loader.loadTexture("flat2.jpg", TEX_FLAT_2, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[TEX_FLAT_x2] = loader.loadTexture("flat_x2.jpg", TEX_FLAT_x2, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[TEX_FLAT_2_x2] = loader.loadTexture("flat2_x2.jpg", TEX_FLAT_2_x2, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[TEX_DIFFUSE_MIX_MAP] = loader.loadTexture("diffuseMixMap.jpg", TEX_DIFFUSE_MIX_MAP, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, false);
  textures[TEX_TERRAIN_NORMAL] = loader.loadTexture("terrainNormal.jpg", TEX_TERRAIN_NORMAL, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[TEX_WATER_NORMAL] = loader.loadTexture("waterNormal.jpg", TEX_WATER_NORMAL, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[TEX_WATER_SPECULAR] = loader.loadTexture("waterSpecular.jpg", TEX_WATER_SPECULAR, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[TEX_HILL] = loader.loadTexture("hill2.jpg", TEX_HILL, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[TEX_HILL_2] = loader.loadTexture("hill.jpg", TEX_HILL_2, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[TEX_HILL_SPECULAR] = loader.loadTexture("hill_specular.jpg", TEX_HILL_SPECULAR, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, false);
  textures[TEX_SHORE] = loader.loadTexture("shore.jpg", TEX_SHORE, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[TEX_UNDERWATER_DIFFUSE] = loader.loadTexture("underwater.jpg", TEX_UNDERWATER_DIFFUSE, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, false);
  textures[TEX_SHORE_2] = loader.loadTexture("shore2.jpg", TEX_SHORE_2, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[TEX_SKYBOX] = loader.loadCubemap("cubemap/", TEX_SKYBOX);
  textures[TEX_FRAME_MULTISAMPLED] = loader.createFrameMSTexture(MULTISAMPLES, TEX_FRAME_MULTISAMPLED);
  textures[TEX_FRAME] = loader.createFrameTexture(TEX_FRAME);
  textures[TEX_FRAME_HDR] = loader.createFrameTexture(TEX_FRAME_HDR);
  textures[TEX_DEPTH_MAP_SUN] = loader.createDepthMapTexture(DEPTH_MAP_TEXTURE_WIDTH, DEPTH_MAP_TEXTURE_HEIGHT, TEX_DEPTH_MAP_SUN);
}

TextureManager::~TextureManager()
{
  for (unsigned int i = 0; i < textures.size(); i++)
    glDeleteTextures(1, &(textures[i]));
}

void TextureManager::createUnderwaterReliefTexture(const std::shared_ptr<WaterGenerator> generator)
{
  textures[TEX_UNDERWATER_RELIEF] = loader.createUnderwaterReliefTexture(generator, TEX_UNDERWATER_RELIEF, GL_LINEAR, GL_LINEAR);
}

GLuint &TextureManager::get(int unit)
{
  return textures[unit];
}
