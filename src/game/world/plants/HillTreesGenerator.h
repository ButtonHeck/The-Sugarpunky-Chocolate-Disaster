#ifndef HILLTREESGENERATOR_H
#define HILLTREESGENERATOR_H
#include "game/world/plants/PlantGenerator.h"

class HillTreesGenerator : public PlantGenerator
{
public:
  HillTreesGenerator();
  void setup(const map2D_f &hillMap,
             const map2D_i &distributionMap);
private:
  void setupMatrices(const map2D_f &hillMap,
                     const map2D_i &distributionMap);
};

#endif // HILLTREESGENERATOR_H
