#ifndef SHORERENDERER_H
#define SHORERENDERER_H
#include <memory>
#include "game/world/terrain/shore/ShoreGenerator.h"

class ShoreRenderer
{
public:
  ShoreRenderer(std::shared_ptr<ShoreGenerator> generator);
  void render();
private:
  std::shared_ptr<ShoreGenerator> generator;
};

#endif // SHORERENDERER_H
