#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include <GL/glew.h>
#include <unordered_map>
#include "TextureLoader.h"
#include "graphics/TextureUnits.h"
#include "generator/WaterGenerator.h"

class TextureManager
{
public:
  TextureManager(TextureLoader& loader);
  virtual ~TextureManager();
  void createUnderwaterReliefTexture(const std::shared_ptr<WaterGenerator> generator);
  GLuint& get(int unit);
private:
  TextureLoader& loader;
  std::unordered_map<int, GLuint> textures;
};

#endif // TEXTUREMANAGER_H
