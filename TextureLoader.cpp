#include "TextureLoader.h"

TextureLoader::TextureLoader()
{
  ilInit();
  ilEnable(IL_ORIGIN_SET);
  ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
}

GLuint TextureLoader::loadTexture(const std::string& path, GLenum wrapType, GLuint textureUnit)
{
  GLuint texture;
  glGenTextures(1, &texture);
  glActiveTexture(GL_TEXTURE0 + textureUnit);
  glBindTexture(GL_TEXTURE_2D, texture);
  if (!ilLoadImage(path.c_str()))
    printf("Error when loading texture", path);
  ILubyte* data = ilGetData();
  auto imageWidth = ilGetInteger(IL_IMAGE_WIDTH);
  auto imageHeight = ilGetInteger(IL_IMAGE_HEIGHT);
  auto imageFormat = ilGetInteger(IL_IMAGE_FORMAT);
  glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, imageWidth, imageHeight, 0, imageFormat, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
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

GLuint TextureLoader::loadTextureArray(const std::string &path, GLenum wrapType, GLuint textureUnit)
{
  GLuint texture;
  glGenTextures(1, &texture);
  glActiveTexture(GL_TEXTURE0 + textureUnit);
  glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
  if (!ilLoadImage(path.c_str()))
    printf("Error when loading texture: ", path);
  ILubyte* data = ilGetData();
  auto imageWidth = ilGetInteger(IL_IMAGE_WIDTH);
  auto imageHeight = ilGetInteger(IL_IMAGE_HEIGHT);
  auto imageFormat = ilGetInteger(IL_IMAGE_FORMAT);
  glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, imageWidth, imageHeight, 4);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,                            imageWidth / 2, imageHeight / 2, imageFormat, GL_UNSIGNED_BYTE, data);
  glTexSubImage2D(GL_TEXTURE_2D, 0, imageWidth / 2, 0,               imageWidth / 2, imageHeight / 2, imageFormat, GL_UNSIGNED_BYTE, data);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, imageHeight / 2,              imageWidth / 2, imageHeight / 2, imageFormat, GL_UNSIGNED_BYTE, data);
  glTexSubImage2D(GL_TEXTURE_2D, 0, imageWidth / 2, imageHeight / 2, imageWidth / 2, imageHeight / 2, imageFormat, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, wrapType);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, wrapType);
  ilDeleteImage(ilGetInteger(IL_ACTIVE_IMAGE));
  return texture;
}

GLuint TextureLoader::createUnderwaterReliefTexture(WaterMapGenerator *waterMapGenerator, GLuint textureUnit)
{
  GLuint underwaterReliefTexture;
  GLubyte* textureData = new GLubyte[TILES_WIDTH * TILES_HEIGHT];
  int left, right, top, bottom, waterCount;
  for (int y = 1; y < TILES_HEIGHT; y++)
    {
      for (int x = 0; x < TILES_WIDTH - 1; x++)
        {
          left = glm::clamp(x - SHORE_SIZE_BASE - 3, 0, TILES_WIDTH - 1);
          right = glm::clamp(x + SHORE_SIZE_BASE + 3, 0, TILES_WIDTH - 1);
          top = glm::clamp(y - SHORE_SIZE_BASE - 3, 1, TILES_HEIGHT);
          bottom = glm::clamp(y + SHORE_SIZE_BASE + 3, 0, TILES_HEIGHT);
          waterCount = 0;
          for (int y1 = top; y1 < bottom; y1++)
            {
              for (int x1 = left; x1 < right; x1++)
                {
                  if (waterMapGenerator->getMap()[y1][x1] != 0 &&
                      waterMapGenerator->getMap()[y1-1][x1] != 0 &&
                      waterMapGenerator->getMap()[y1-1][x1+1] != 0 &&
                      waterMapGenerator->getMap()[y1][x1+1] != 0)
                    ++waterCount;
                }
            }
          textureData[y * TILES_WIDTH + x] = (GLubyte)waterCount;
        }
    }
  glGenTextures(1, &underwaterReliefTexture);
  glActiveTexture(GL_TEXTURE0 + textureUnit);
  glBindTexture(GL_TEXTURE_2D, underwaterReliefTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, TILES_WIDTH, TILES_HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, textureData);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  delete[] textureData;
  return underwaterReliefTexture;
}

