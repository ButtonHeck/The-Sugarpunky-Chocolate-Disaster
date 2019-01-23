#ifndef LANDPLANTSGENERATOR_H
#define LANDPLANTSGENERATOR_H
#include "game/world/models/plants/PlantGenerator.h"

class LandPlantsGenerator : public PlantGenerator
{
public:
  LandPlantsGenerator();
  void setup(const map2D_f &baseMap,
             const map2D_f &hillMap,
             const map2D_i &distributionMap);
private:
  void setupMatrices(const map2D_f &baseMap, const map2D_f &hillMap, const map2D_i &distributionMap);
};

#endif // LANDPLANTSGENERATOR_H
