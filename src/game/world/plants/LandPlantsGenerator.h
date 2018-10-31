#ifndef LANDPLANTSGENERATOR_H
#define LANDPLANTSGENERATOR_H
#include "game/world/plants/PlantGenerator.h"

class LandPlantsGenerator : public PlantGenerator
{
public:
  LandPlantsGenerator();
  void setup(map2D_f& baseMap,
             map2D_f& hillMap,
             map2D_i& distributionMap);
private:
  void setupMatrices(map2D_f& baseMap, map2D_f& hillMap, map2D_i &distributionMap);
};

#endif // LANDPLANTSGENERATOR_H
