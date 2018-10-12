#ifndef FONTLOADER_H
#define FONTLOADER_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <IL/il.h>
#include <map>
#include "graphics/TextureUnits.h"

struct Character {
  int asciiCode, x, y, width, height, xoffset, yoffset, xadvance;
};

class FontLoader
{
public:
  FontLoader(const std::string& fontFile, const std::string& fontTexture);
  virtual ~FontLoader() = default;
  const std::map<char, Character>& getAlphabet() const;
  GLuint getTextureWidth() const;
  GLuint getTextureHeight() const;
private:
  std::map<char, Character> alphabet;
  GLuint fontTexture, textureWidth, textureHeight;
};

#endif // FONTLOADER_H
