#ifndef TREESRENDERER_H
#define TREESRENDERER_H
#include "game/world/models/Model.h"

class TreesRenderer
{
public:
  TreesRenderer() = default;
  void render(std::vector<Model> &landModels, std::vector<Model> &hillModels, bool isShadow);
};

#endif // TREESRENDERER_H
