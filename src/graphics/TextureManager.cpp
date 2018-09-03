#include "TextureManager.h"

TextureManager::TextureManager(TextureLoader &loader, int scr_width, int scr_height)
  :
    loader(loader)
{
  textures[FLAT] = loader.loadTexture("/textures/flat.jpg", FLAT, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[FLAT_2] = loader.loadTexture("/textures/flat2.jpg", FLAT_2, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[FLAT_x2] = loader.loadTexture("/textures/flat_x2.jpg", FLAT_x2, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[FLAT_2_x2] = loader.loadTexture("/textures/flat2_x2.jpg", FLAT_2_x2, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[DIFFUSE_MIX_MAP] = loader.loadTexture("/textures/diffuseMixMap.jpg", DIFFUSE_MIX_MAP, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, false);
  textures[TERRAIN_NORMAL] = loader.loadTexture("/textures/terrainNormal.jpg", TERRAIN_NORMAL, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[HILL] = loader.loadTexture("/textures/hill2.jpg", HILL, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[HILL_2] = loader.loadTexture("/textures/hill.jpg", HILL_2, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[HILL_SPECULAR] = loader.loadTexture("/textures/hill_specular.jpg", HILL_SPECULAR, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, false);
  textures[SHORE] = loader.loadTexture("/textures/shore.jpg", SHORE, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[SHORE_2] = loader.loadTexture("/textures/shore2.jpg", SHORE_2, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
  textures[SKYBOX] = loader.loadCubemap("/textures/cubemap/", SKYBOX);
  textures[FRAME_MS_TEXTURE] = loader.createFrameMSTexture(scr_width, scr_height, MULTISAMPLES, FRAME_MS_TEXTURE);
  textures[FRAME_TEXTURE] = loader.createFrameTexture(scr_width, scr_height, FRAME_TEXTURE);
  textures[FRAME_HDR_TEXTURE] = loader.createFrameTexture(scr_width, scr_height, FRAME_HDR_TEXTURE);
  textures[DEPTH_MAP_SUN] = loader.createDepthMapTexture(DEPTH_MAP_TEXTURE_WIDTH, DEPTH_MAP_TEXTURE_HEIGHT, DEPTH_MAP_SUN);
  textures[DEPTH_MAP_CAMERA] = loader.createDepthMapTexture(scr_width, scr_height, DEPTH_MAP_CAMERA);
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
