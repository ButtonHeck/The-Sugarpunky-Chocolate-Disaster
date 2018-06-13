#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include <GL/glew.h>
#include <unordered_map>
#include "TextureLoader.h"
#include "src/generator/WaterMapGenerator.h"

class TextureManager
{
public:
  TextureManager(TextureLoader& loader);
  void createUnderwaterReliefTexture(WaterMapGenerator* generator);
  GLuint& getTexture(int unit);
  void deleteTextures();
private:
  TextureLoader& loader;
  std::unordered_map<int, GLuint> textures;
};

#endif // TEXTUREMANAGER_H
