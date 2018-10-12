#include "graphics/TextureLoader.h"

TextureLoader::TextureLoader(ScreenResolution &screen)
  :
    screenResolution(screen)
{
  ilInit();
  ilEnable(IL_ORIGIN_SET);
  ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
}

GLuint TextureLoader::createAndBindTextureObject(GLenum target, GLuint textureUnit)
{
  GLuint texture;
  glCreateTextures(target, 1, &texture);
  glActiveTexture(GL_TEXTURE0 + textureUnit);
  glBindTexture(target, texture);
  return texture;
}

GLuint TextureLoader::loadTexture(const std::string& path, GLuint textureUnit, GLenum wrapType, GLint magFilter, GLint minFilter, bool useAnisotropy, bool includeCWD)
{
  GLuint texture = createAndBindTextureObject(GL_TEXTURE_2D, textureUnit);
  std::string fullPath = includeCWD ? std::string(TEXTURES_DIR + path) : path;
  if (!ilLoadImage(fullPath.c_str()))
    printf("Error when loading texture: %s\n", fullPath.c_str());
  ILubyte* data = ilGetData();
  auto imageWidth = ilGetInteger(IL_IMAGE_WIDTH);
  auto imageHeight = ilGetInteger(IL_IMAGE_HEIGHT);
  auto imageChannels = ilGetInteger(IL_IMAGE_CHANNELS);
  GLenum internalFormat, dataFormat;
  if (imageChannels == 4)
    {
      internalFormat = HDR_ENABLED ? GL_SRGB8_ALPHA8 : GL_RGBA8;
      dataFormat = GL_RGBA;
    }
  else if (imageChannels == 3)
    {
      internalFormat = HDR_ENABLED ? GL_SRGB8 : GL_RGB8;
      dataFormat = GL_RGB;
    }
  GLuint mipLevel = getMaxMip(imageWidth, imageHeight);
  glTextureStorage2D(texture, mipLevel, internalFormat, imageWidth, imageHeight);
  glTextureSubImage2D(texture, 0, 0, 0, imageWidth, imageHeight, dataFormat, GL_UNSIGNED_BYTE, data);
  glGenerateTextureMipmap(texture);
  setTexture2DParameters(magFilter, minFilter, wrapType);
  if (useAnisotropy)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, ANISOTROPY);
  ilDeleteImage(ilGetInteger(IL_ACTIVE_IMAGE));
  return texture;
}

GLuint TextureLoader::createFrameMSTexture(int samples, GLuint textureUnit)
{
  GLuint texture = createAndBindTextureObject(GL_TEXTURE_2D_MULTISAMPLE, textureUnit);
  glTextureStorage2DMultisample(texture, samples, GL_RGB8, screenResolution.getWidth(), screenResolution.getHeight(), GL_TRUE);
  return texture;
}

GLuint TextureLoader::createFrameTexture(GLuint textureUnit)
{
  GLuint texture = createAndBindTextureObject(GL_TEXTURE_2D, textureUnit);
  glTextureStorage2D(texture, 1, GL_RGB8, screenResolution.getWidth(), screenResolution.getHeight());
  return texture;
}

GLuint TextureLoader::createDepthMapTexture(int width, int height, GLuint textureUnit)
{
  GLuint texture = createAndBindTextureObject(GL_TEXTURE_2D, textureUnit);
  glTextureStorage2D(texture, 1, GL_DEPTH_COMPONENT16, width, height);
  setTexture2DParameters(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_BORDER);
  float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
  return texture;
}

GLuint TextureLoader::loadCubemap(const std::string& directory, GLuint textureUnit)
{
  std::vector<std::string> faces;
  faces.assign(
  {
          TEXTURES_DIR + directory + "right.png",
          TEXTURES_DIR + directory + "left.png",
          TEXTURES_DIR + directory + "up.png",
          TEXTURES_DIR + directory + "down.png",
          TEXTURES_DIR + directory + "back.png",
          TEXTURES_DIR + directory + "front.png"
        });
  ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
  GLuint texture = createAndBindTextureObject(GL_TEXTURE_CUBE_MAP, textureUnit);
  for (unsigned int i = 0; i < faces.size(); i++)
    {
      if (!ilLoadImage(faces[i].c_str()))
        printf("Error while loading cubemap element: %s\n", faces[i].c_str());
      auto format = ilGetInteger(IL_IMAGE_FORMAT);
      auto width = ilGetInteger(IL_IMAGE_WIDTH);
      auto height = ilGetInteger(IL_IMAGE_HEIGHT);
      ILubyte* data = ilGetData();
      GLenum internalFormat = HDR_ENABLED ? GL_SRGB8 : GL_RGB8;
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      ilDeleteImage(ilGetInteger(IL_ACTIVE_IMAGE));
    }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  return texture;
}

GLuint TextureLoader::createUnderwaterReliefTexture(const std::shared_ptr<WaterMapGenerator> waterMapGenerator, GLuint textureUnit, GLint magFilter, GLint minFilter)
{
  static bool needStorage = true;
  static GLuint texture;
  std::unique_ptr<GLubyte[]> textureData(new GLubyte[WORLD_WIDTH * WORLD_HEIGHT]);
  int left, right, top, bottom;
  float waterCount;
  for (int y = 1; y < WORLD_HEIGHT; y++)
    {
      for (int x = 0; x < WORLD_WIDTH - 1; x++)
        {
          left = glm::clamp(x - SHORE_SIZE_BASE - 3, 0, WORLD_WIDTH - 1);
          right = glm::clamp(x + SHORE_SIZE_BASE + 3, 0, WORLD_WIDTH - 1);
          top = glm::clamp(y - SHORE_SIZE_BASE - 3, 1, WORLD_HEIGHT);
          bottom = glm::clamp(y + SHORE_SIZE_BASE + 3, 0, WORLD_HEIGHT);
          waterCount = 0;
          for (int y1 = top; y1 < bottom; y1++)
            {
              for (int x1 = left; x1 < right; x1++)
                {
                  if (waterMapGenerator->getMap()[y1][x1] != 0 &&
                      waterMapGenerator->getMap()[y1-1][x1] != 0 &&
                      waterMapGenerator->getMap()[y1-1][x1+1] != 0 &&
                      waterMapGenerator->getMap()[y1][x1+1] != 0)
                    waterCount += 1.0 - ((SHORE_SIZE_BASE - 4) * 0.25);
                }
            }
          textureData[y * WORLD_WIDTH + x] = (GLubyte)waterCount;
        }
    }
  if (needStorage)
    {
      glCreateTextures(GL_TEXTURE_2D, 1, &texture);
      glTextureStorage2D(texture, 1, GL_R8, WORLD_WIDTH, WORLD_HEIGHT);
      needStorage = false;
    }
  glActiveTexture(GL_TEXTURE0 + textureUnit);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTextureSubImage2D(texture, 0, 0, 0, WORLD_WIDTH, WORLD_HEIGHT, GL_RED, GL_UNSIGNED_BYTE, textureData.get());
  setTexture2DParameters(magFilter, minFilter, GL_REPEAT);
  return texture;
}

unsigned int TextureLoader::getMaxMip(unsigned int width, unsigned int height)
{
  unsigned int refDimension = std::max(width, height);
  unsigned int mip = 1;
  while (refDimension / 2 != 0)
    {
      ++mip;
      refDimension /= 2;
    }
  return mip;
}

void TextureLoader::setTexture2DParameters(GLint magFilter, GLint minFilter, GLenum wrapType)
{
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapType);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapType);
}
