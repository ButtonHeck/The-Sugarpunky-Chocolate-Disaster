#include "TextureManager.h"

TextureManager::TextureManager(TextureLoader& loader)
  :
    loader(loader)
{
  textures[FLAT] = loader.loadTexture("flat.jpg", FLAT, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[FLAT_2] = loader.loadTexture("flat2.jpg", FLAT_2, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[FLAT_x2] = loader.loadTexture("flat_x2.jpg", FLAT_x2, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[FLAT_2_x2] = loader.loadTexture("flat2_x2.jpg", FLAT_2_x2, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[DIFFUSE_MIX_MAP] = loader.loadTexture("diffuseMixMap.jpg", DIFFUSE_MIX_MAP, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, false);
  textures[TERRAIN_NORMAL] = loader.loadTexture("terrainNormal.jpg", TERRAIN_NORMAL, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[WATER_NORMAL] = loader.loadTexture("waterNormal.jpg", WATER_NORMAL, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[WATER_SPECULAR] = loader.loadTexture("waterSpecular.jpg", WATER_SPECULAR, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[HILL] = loader.loadTexture("hill2.jpg", HILL, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[HILL_2] = loader.loadTexture("hill.jpg", HILL_2, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[HILL_SPECULAR] = loader.loadTexture("hill_specular.jpg", HILL_SPECULAR, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, false);
  textures[SHORE] = loader.loadTexture("shore.jpg", SHORE, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[UNDERWATER_DIFFUSE] = loader.loadTexture("underwater.jpg", UNDERWATER_DIFFUSE, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, false);
  textures[SHORE_2] = loader.loadTexture("shore2.jpg", SHORE_2, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[SKYBOX] = loader.loadCubemap("cubemap/", SKYBOX);
  textures[FRAME_MS_TEXTURE] = loader.createFrameMSTexture(MULTISAMPLES, FRAME_MS_TEXTURE);
  textures[FRAME_TEXTURE] = loader.createFrameTexture(FRAME_TEXTURE);
  textures[FRAME_HDR_TEXTURE] = loader.createFrameTexture(FRAME_HDR_TEXTURE);
  textures[DEPTH_MAP_SUN] = loader.createDepthMapTexture(DEPTH_MAP_TEXTURE_WIDTH, DEPTH_MAP_TEXTURE_HEIGHT, DEPTH_MAP_SUN);
}

void TextureManager::createUnderwaterReliefTexture(WaterMapGenerator *generator)
{
  textures[UNDERWATER_RELIEF] = loader.createUnderwaterReliefTexture(generator, UNDERWATER_RELIEF, GL_LINEAR, GL_LINEAR);
}

GLuint &TextureManager::get(int unit)
{
  return textures[unit];
}

void TextureManager::deleteTextures()
{
  for (unsigned int i = 0; i < textures.size(); i++)
    glDeleteTextures(1, &(textures[i]));
}
