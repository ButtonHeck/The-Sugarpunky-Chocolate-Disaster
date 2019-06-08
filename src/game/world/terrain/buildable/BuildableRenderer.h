#ifndef BUIDABLERENDERER_H
#define BUIDABLERENDERER_H
#include "BuildableGenerator"

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
