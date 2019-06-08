#ifndef UNDERWATERRENDERER_H
#define UNDERWATERRENDERER_H
#include "UnderwaterGenerator"

class UnderwaterRenderer
{
public:
  UnderwaterRenderer(UnderwaterGenerator& generator);
  void render();
private:
  UnderwaterGenerator& generator;
};

#endif // UNDERWATERRENDERER_H
