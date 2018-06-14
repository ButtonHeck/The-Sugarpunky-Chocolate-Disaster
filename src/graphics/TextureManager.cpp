#include "TextureManager.h"

TextureManager::TextureManager(TextureLoader &loader, int width, int height)
  :
    loader(loader)
{
  textures[FLAT] = loader.loadTexture("/textures/flat.jpg", FLAT, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
  textures[FLAT_2] = loader.loadTexture("/textures/flat2.jpg", FLAT_2, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
  textures[FLAT_x2] = loader.loadTexture("/textures/flat_x2.jpg", FLAT_x2, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
  textures[FLAT_2_x2] = loader.loadTexture("/textures/flat2_x2.jpg", FLAT_2_x2, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
  textures[NORMAL_MAP] = loader.loadTexture("/textures/normalMap.jpg", NORMAL_MAP, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
  textures[HILL] = loader.loadTexture("/textures/hill.jpg", HILL, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
  textures[HILL_2] = loader.loadTexture("/textures/hill2.jpg", HILL_2, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
  textures[HILL_SPECULAR] = loader.loadTexture("/textures/hill_specular.jpg", HILL_SPECULAR, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
  textures[SHORE] = loader.loadTexture("/textures/shore.jpg", SHORE, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
  textures[SHORE_2] = loader.loadTexture("/textures/shore2.jpg", SHORE_2, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
  textures[UNDERWATER] = loader.loadTexture("/textures/underwater_sand.jpg", UNDERWATER, GL_REPEAT, GL_NEAREST, GL_LINEAR_MIPMAP_LINEAR);
  textures[WATER] = loader.loadTexture("/textures/water.png", WATER, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
  textures[WATER_SPECULAR] = loader.loadTexture("/textures/water_specular.png", WATER_SPECULAR, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
  textures[SKYBOX] = loader.loadCubemap("/textures/cubemap/", SKYBOX);
  textures[FRAME_HDR_TEXTURE] = loader.createFrameHDRTexture(width, height, FRAME_HDR_TEXTURE);
  textures[FRAME_MS_TEXTURE] = loader.createFrameMSTexture(width, height, MULTISAMPLES, FRAME_MS_TEXTURE);
  textures[FRAME_TEXTURE] = loader.createFrameTexture(width, height, FRAME_TEXTURE);
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
