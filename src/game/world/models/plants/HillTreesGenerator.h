#ifndef HILLTREESGENERATOR_H
#define HILLTREESGENERATOR_H
#include "game/world/models/plants/PlantGenerator.h"

class HillTreesGenerator : public PlantGenerator
{
public:
  HillTreesGenerator();
  void setup(const map2D_f &hillMap, const map2D_i &distributionMap, const map2D_vec3 &hillsNormalMap);

private:
  const float MIN_SCALE_TREES = 0.38f;
  const float MAX_SCALE_TREES = 0.51f;
  const float MIN_SCALE_ROCKS = MIN_SCALE_TREES * 0.75f;
  const float MAX_SCALE_ROCKS = MAX_SCALE_TREES * 0.75f;
  const float MIN_POSITION_OFFSET = -0.6f;
  const float MAX_POSITION_OFFSET = 0.6f;
  const float MIN_ROTATION_OFFSET = -0.05f;
  const float MAX_ROTATION_OFFSET = 0.05f;
  const float MAX_SURFACE_SLOPE_FOR_TREES = 1.0f;
  const float MAX_SURFACE_SLOPE_FOR_ROCKS = 2.6f;
  void setupMatrices(const map2D_f &hillMap, const map2D_i &distributionMap, const map2D_vec3 &hillsNormalMap);
  size_t numSurfaceOrientedModels;
};

#endif // HILLTREESGENERATOR_H
