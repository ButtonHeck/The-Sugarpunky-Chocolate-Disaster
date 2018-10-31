#ifndef GRASSGENERATOR_H
#define GRASSGENERATOR_H
#include "game/world/plants/PlantGenerator.h"

class GrassGenerator : public PlantGenerator
{
public:
  GrassGenerator();
  void setup(map2D_f& baseMap,
             map2D_f& hillMap,
             map2D_i& distributionMap);
private:
  void setupMatrices(map2D_f& baseMap, map2D_f& hillMap, map2D_i &distributionMap);
};

#endif // GRASSGENERATOR_H
