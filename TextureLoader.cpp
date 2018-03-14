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

GLuint TextureLoader::loadCubemap(std::vector<std::string> &faces)
{
  ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
  for (unsigned int i = 0; i < faces.size(); i++)
    {
      if (!ilLoadImage(faces[i].c_str()))
        printf("Error while loading cubemap element\n", faces[i].c_str());
      auto format = ilGetInteger(IL_IMAGE_FORMAT);
      auto width = ilGetInteger(IL_IMAGE_WIDTH);
      auto height = ilGetInteger(IL_IMAGE_HEIGHT);
      ILubyte* data = ilGetData();
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      ilDeleteImage(ilGetInteger(IL_ACTIVE_IMAGE));
    }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  return texture;
}

