#include "TextureLoader.h"

TextureLoader::TextureLoader()
{
  ilInit();
  ilEnable(IL_ORIGIN_SET);
  ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
}

GLuint TextureLoader::loadTexture(const std::string& path, GLenum wrapType)
{
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  if (!ilLoadImage(path.c_str()))
    printf("Error when loading grass texture\n");
  ILubyte* data = ilGetData();
  auto imageWidth = ilGetInteger(IL_IMAGE_WIDTH);
  auto imageHeight = ilGetInteger(IL_IMAGE_HEIGHT);
  auto imageFormat = ilGetInteger(IL_IMAGE_FORMAT);
  glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, imageWidth, imageHeight, 0, imageFormat, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapType);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapType);
  ilDeleteImage(ilGetInteger(IL_ACTIVE_IMAGE));
  return texture;
}

