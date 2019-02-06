#include "graphics/textures/TextureLoader.h"

TextureLoader::TextureLoader(ScreenResolution &screen)
  :
    screenResolution(screen)
{
  ilInit();
  ilEnable(IL_ORIGIN_SET);
  ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
}

GLuint TextureLoader::createTextureObject(GLenum target, GLuint textureUnit, bool isBindless)
{
  GLuint texture;
  glCreateTextures(target, 1, &texture);
  if (!isBindless)
    {
      glActiveTexture(GL_TEXTURE0 + textureUnit);
      glBindTexture(target, texture);
    }
  return texture;
}

GLuint TextureLoader::loadTexture(const std::string& path,
                                  GLuint textureUnit,
                                  GLenum wrapType,
                                  GLint magFilter,
                                  GLint minFilter,
                                  bool useAnisotropy,
                                  bool includeCWD,
                                  bool isBindless,
                                  bool explicitNoSRGB)
{
  GLuint texture = createTextureObject(GL_TEXTURE_2D, textureUnit, isBindless);
  std::string fullPath = includeCWD ? std::string(TEXTURES_DIR + path) : path;
  if (!ilLoadImage(fullPath.c_str()))
    Logger::log("Error when loading texture: %\n", fullPath.c_str());
  ILubyte* data = ilGetData();
  auto imageWidth = ilGetInteger(IL_IMAGE_WIDTH);
  auto imageHeight = ilGetInteger(IL_IMAGE_HEIGHT);
  auto imageChannels = ilGetInteger(IL_IMAGE_CHANNELS);
  GLenum internalFormat, dataFormat;
  if (imageChannels == 4)
    {
      internalFormat = explicitNoSRGB ? GL_RGBA8 : (HDR_ENABLED ? GL_SRGB8_ALPHA8 : GL_RGBA8);
      dataFormat = GL_RGBA;
    }
  else if (imageChannels == 3)
    {
      internalFormat = explicitNoSRGB ? GL_RGB8 : (HDR_ENABLED ? GL_SRGB8 : GL_RGB8);
      dataFormat = GL_RGB;
    }
  else
    throw std::invalid_argument("Could not handle image with: " + std::to_string(imageChannels) + " channels");
  GLsizei mipLevel = ((GLsizei)log2(std::max(imageWidth, imageHeight)) + 1);
  glTextureStorage2D(texture, mipLevel, internalFormat, imageWidth, imageHeight);
  glTextureSubImage2D(texture, 0, 0, 0, imageWidth, imageHeight, dataFormat, GL_UNSIGNED_BYTE, data);
  glGenerateTextureMipmap(texture);
  setTexture2DParameters(texture, magFilter, minFilter, wrapType);
  if (useAnisotropy)
    glTextureParameterf(texture, GL_TEXTURE_MAX_ANISOTROPY, ANISOTROPY);
  ilDeleteImage(ilGetInteger(IL_ACTIVE_IMAGE));
  return texture;
}

GLuint TextureLoader::createFrameMSTexture(int samples, GLuint textureUnit)
{
  GLuint texture = createTextureObject(GL_TEXTURE_2D_MULTISAMPLE, textureUnit, false);
  glTextureStorage2DMultisample(texture, samples, GL_RGB16, screenResolution.getWidth(), screenResolution.getHeight(), GL_TRUE);
  return texture;
}

GLuint TextureLoader::createFrameTexture(GLuint textureUnit, bool isDepthTexture, bool useAnisotropy)
{
  GLuint texture = createTextureObject(GL_TEXTURE_2D, textureUnit, false);
  GLenum format = isDepthTexture ? GL_DEPTH_COMPONENT24 : GL_RGB16;
  glTextureStorage2D(texture, 1, format, screenResolution.getWidth(), screenResolution.getHeight());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  if (useAnisotropy)
    glTextureParameterf(texture, GL_TEXTURE_MAX_ANISOTROPY, ANISOTROPY);
  return texture;
}

GLuint TextureLoader::createFrameTextureSized(GLuint textureUnit, bool isDepthTexture, int width, int height, bool useAnisotropy)
{
  GLuint texture = createTextureObject(GL_TEXTURE_2D, textureUnit, false);
  GLenum format = isDepthTexture ? GL_DEPTH_COMPONENT24 : GL_RGB16;
  glTextureStorage2D(texture, 1, format, width, height);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  if (useAnisotropy)
    glTextureParameterf(texture, GL_TEXTURE_MAX_ANISOTROPY, ANISOTROPY);
  return texture;
}

GLuint TextureLoader::createDepthMapTexture(int width, int height, GLuint textureUnit)
{
  GLuint texture = createTextureObject(GL_TEXTURE_2D_ARRAY, textureUnit, false);
  glTextureStorage3D(texture, 3, GL_DEPTH_COMPONENT24, width, height, 3);
  GLfloat borderColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
  glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  setTex2DArrayParameters(GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER);
  return texture;
}

GLuint TextureLoader::loadCubemap(const std::string& directory, GLuint textureUnit, bool explicitNoSRGB)
{
  std::vector<std::string> faces;
  faces.assign(
  {
          TEXTURES_DIR + directory + "right.png",
          TEXTURES_DIR + directory + "left.png",
          TEXTURES_DIR + directory + "up.png",
          TEXTURES_DIR + directory + "down.png",
          TEXTURES_DIR + directory + "front.png",
          TEXTURES_DIR + directory + "back.png"
        });
  ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
  GLuint texture = createTextureObject(GL_TEXTURE_CUBE_MAP, textureUnit, false);
  for (unsigned int i = 0; i < faces.size(); i++)
    {
      if (!ilLoadImage(faces[i].c_str()))
        Logger::log("Error while loading cubemap element: %\n", faces[i].c_str());
      auto format = ilGetInteger(IL_IMAGE_FORMAT);
      auto width = ilGetInteger(IL_IMAGE_WIDTH);
      auto height = ilGetInteger(IL_IMAGE_HEIGHT);
      ILubyte* data = ilGetData();
      GLenum internalFormat = explicitNoSRGB ? GL_RGBA8 : (HDR_ENABLED ? GL_SRGB8_ALPHA8 : GL_RGBA8);
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

GLuint TextureLoader::createUnderwaterReliefTexture(const map2D_f &waterMap, GLuint textureUnit, GLint magFilter, GLint minFilter)
{
  static bool needStorage = true;
  static GLuint texture;
  GLubyte textureData[WORLD_WIDTH * WORLD_HEIGHT] = {0};
  int left, right, top, bottom;
  float waterCount;
  for (int y = 1; y < WORLD_HEIGHT; y++)
    {
      for (int x = 0; x < WORLD_WIDTH - 1; x++)
        {
          left = glm::clamp(x - RIVER_WIDTH_DEFAULT - 3, 0, WORLD_WIDTH - 1);
          right = glm::clamp(x + RIVER_WIDTH_DEFAULT + 3, 0, WORLD_WIDTH - 1);
          top = glm::clamp(y - RIVER_WIDTH_DEFAULT - 3, 1, WORLD_HEIGHT);
          bottom = glm::clamp(y + RIVER_WIDTH_DEFAULT + 3, 0, WORLD_HEIGHT);
          waterCount = 0;
          for (int y1 = top; y1 < bottom; y1++)
            {
              for (int x1 = left; x1 < right; x1++)
                {
                  if (waterMap[y1][x1] != 0 &&
                      waterMap[y1-1][x1] != 0 &&
                      waterMap[y1-1][x1+1] != 0 &&
                      waterMap[y1][x1+1] != 0)
                    waterCount += 1.0 - ((RIVER_WIDTH_DEFAULT - 4) * 0.25);
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
  glTextureSubImage2D(texture, 0, 0, 0, WORLD_WIDTH, WORLD_HEIGHT, GL_RED, GL_UNSIGNED_BYTE, textureData);
  setTexture2DParameters(texture, magFilter, minFilter, GL_REPEAT);
  return texture;
}

void TextureLoader::setTexture2DParameters(GLuint texture, GLint magFilter, GLint minFilter, GLenum wrapType)
{
  glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, magFilter);
  glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, minFilter);
  glTextureParameteri(texture, GL_TEXTURE_WRAP_S, wrapType);
  glTextureParameteri(texture, GL_TEXTURE_WRAP_T, wrapType);
}

void TextureLoader::setTex2DArrayParameters(GLint magFilter, GLint minFilter, GLenum wrapType)
{
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, magFilter);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, minFilter);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, wrapType);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, wrapType);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, wrapType);
}
