#pragma once

#include "TextureLoader"
#include "TextureUnits"

#include <GL/glew.h>
#include <unordered_map>

class TextureManager
{
public:
  TextureManager(TextureLoader& loader);
  virtual ~TextureManager();
  void createUnderwaterReliefTexture(const map2D_f &waterMap);
  GLuint& get(int textureUnit);
  TextureLoader& getLoader();

private:
  TextureLoader& loader;
  std::unordered_map<int, GLuint> textures;
};
