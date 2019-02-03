#ifndef BUIDABLERENDERER_H
#define BUIDABLERENDERER_H
#include "game/world/terrain/buildable/BuildableGenerator.h"

class BuildableRenderer
{
public:
  BuildableRenderer(BuildableGenerator& generator);
  void renderBuildable();
  void renderSelected();
private:
  BuildableGenerator& generator;
};

#endif // BUIDABLERENDERER_H
