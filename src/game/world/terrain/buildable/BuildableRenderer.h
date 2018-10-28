#ifndef BUIDABLERENDERER_H
#define BUIDABLERENDERER_H
#include <memory>
#include "game/world/terrain/buildable/BuildableGenerator.h"

class BuildableRenderer
{
public:
  BuildableRenderer(std::shared_ptr<BuildableGenerator> generator);
  void renderBuildable();
  void renderSelected();
private:
  std::shared_ptr<BuildableGenerator> generator;
};

#endif // BUIDABLERENDERER_H
