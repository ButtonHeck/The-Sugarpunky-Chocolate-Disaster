#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include <GL/glew.h>
#include <unordered_map>
#include "TextureLoader.h"
#include "game/TextureUnits.h"
#include "generator/WaterMapGenerator.h"

class TextureManager
{
public:
  TextureManager(TextureLoader& loader);
  void createUnderwaterReliefTexture(WaterMapGenerator* generator);
  GLuint& get(int unit);
  void deleteTextures();
private:
  TextureLoader& loader;
  std::unordered_map<int, GLuint> textures;
};

#endif // TEXTUREMANAGER_H
