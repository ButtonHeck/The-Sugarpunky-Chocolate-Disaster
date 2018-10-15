#ifndef FONTLOADER_H
#define FONTLOADER_H
#include <iosfwd>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <IL/il.h>
#include <unordered_map>
#include "graphics/TextureUnits.h"

struct Character {
  int asciiCode, x, y, width, height, xoffset, yoffset, xadvance;
};

class FontLoader
{
public:
  FontLoader(const std::string& fontFile, const std::string& fontTexture);
  virtual ~FontLoader() = default;
  std::unordered_map<char, Character>& getAlphabet();
  GLuint getTextureWidth() const;
  GLuint getTextureHeight() const;
private:
  std::unordered_map<char, Character> alphabet;
  GLuint fontTexture, textureWidth, textureHeight;
};

#endif // FONTLOADER_H
