#ifndef LANDRENDERER_H
#define LANDRENDERER_H
#include <memory>
#include "game/world/terrain/land/LandGenerator.h"
#include "graphics/Frustum.h"
#include "util/BenchmarkTimer.h"
#include "graphics/TextureUnits.h"

class LandRenderer
{
public:
  LandRenderer(LandGenerator& generator);
  void render(Frustum& frustum, GLuint& texture);
private:
  void addIndirectBufferData(GLuint& primCount,
                             GLuint* buffer,
                             GLuint& dataOffset,
                             GLuint numInstances,
                             GLuint instanceOffset);

  LandGenerator& generator;
};

#endif // LANDRENDERER_H
