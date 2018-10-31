#ifndef HILLTREESGENERATOR_H
#define HILLTREESGENERATOR_H
#include "game/world/plants/PlantGenerator.h"

class HillTreesGenerator : public PlantGenerator
{
public:
  HillTreesGenerator();
  void setup(map2D_f& hillMap,
             map2D_i& distributionMap);
private:
  void setupMatrices(map2D_f& hillMap,
                     map2D_i& distributionMap);
};

#endif // HILLTREESGENERATOR_H
