#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H
#include <IL/il.h>
#include <GL/glew.h>
#include <string>
#include <vector>
#include "Settings.h"
#include "WaterMapGenerator.h"

class TextureLoader
{
public:
  TextureLoader();
  GLuint loadTexture(const std::string& path, GLenum wrapType);
  GLuint loadCubemap(std::vector<std::string>& faces);
  GLuint createUnderwaterReliefTexture(WaterMapGenerator* waterMapGenerator, GLuint textureUnit);
};

#endif // TEXTURELOADER_H
