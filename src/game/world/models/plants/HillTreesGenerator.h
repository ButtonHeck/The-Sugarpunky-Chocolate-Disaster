#ifndef HILLTREESGENERATOR_H
#define HILLTREESGENERATOR_H
#include "game/world/models/plants/PlantGenerator.h"

class HillTreesGenerator : public PlantGenerator
{
public:
  HillTreesGenerator();
  void setup(const map2D_f &hillMap,
             const map2D_i &distributionMap,
             const map2D_vec3 &hillsNormalMap);
private:
  void setupMatrices(const map2D_f &hillMap,
                     const map2D_i &distributionMap,
                     const map2D_vec3 &hillsNormalMap);
  size_t numSurfaceModels;
};

#endif // HILLTREESGENERATOR_H
