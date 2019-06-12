#pragma once

#include <GL/glew.h>
#include <unordered_map>

struct Character {
  int asciiCode, x, y, width, height, xoffset, yoffset, xadvance;
};

class FontLoader
{
public:
  FontLoader(const std::string& fontFile, const std::string& fontTexture);
  virtual ~FontLoader() = default;
  std::unordered_map<char, Character>& getAlphabet() noexcept;
  GLuint getTextureWidth() const noexcept;
  GLuint getTextureHeight() const noexcept;

private:
  std::unordered_map<char, Character> alphabet;
  GLuint fontTexture, textureWidth, textureHeight;
};
