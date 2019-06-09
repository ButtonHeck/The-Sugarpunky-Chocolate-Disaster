#pragma once

#include "TextureUnits"
#include "TypeAliases"

#include <GL/glew.h>
#include <unordered_map>

class TextureLoader;

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
