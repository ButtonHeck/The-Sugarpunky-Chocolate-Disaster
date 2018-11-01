#ifndef THESUNRENDERER_H
#define THESUNRENDERER_H
#include "game/world/ambience/theSun/TheSun.h"
#include "graphics/RendererStateManager.h"
#include "util/BenchmarkTimer.h"

class TheSunRenderer
{
public:
  TheSunRenderer(TheSun& theSun);
  void render();
private:
  TheSun& theSun;
};

#endif // THESUNRENDERER_H
