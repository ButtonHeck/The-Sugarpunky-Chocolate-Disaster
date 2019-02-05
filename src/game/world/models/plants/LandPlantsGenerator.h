#ifndef LANDPLANTSGENERATOR_H
#define LANDPLANTSGENERATOR_H
#include "game/world/models/plants/PlantGenerator.h"

class LandPlantsGenerator : public PlantGenerator
{
public:
  LandPlantsGenerator();
  void setup(const map2D_f &landMap, const map2D_f &hillMap, const map2D_i &distributionMap);

private:
  const float MIN_SCALE = 0.27f;
  const float MAX_SCALE = 0.32f;
  const float MIN_POSITION_OFFSET = -0.25f;
  const float MAX_POSITION_OFFSET = 0.25f;
  void setupMatrices(const map2D_f &landMap, const map2D_f &hillMap, const map2D_i &distributionMap);
};

#endif // LANDPLANTSGENERATOR_H
