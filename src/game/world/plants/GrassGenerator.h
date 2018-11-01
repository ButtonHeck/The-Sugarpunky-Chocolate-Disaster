#ifndef GRASSGENERATOR_H
#define GRASSGENERATOR_H
#include "game/world/plants/PlantGenerator.h"

class GrassGenerator : public PlantGenerator
{
public:
  GrassGenerator();
  void setup(const map2D_f &baseMap,
             const map2D_f &hillMap,
             const map2D_i &distributionMap);
private:
  void setupMatrices(const map2D_f &baseMap, const map2D_f &hillMap, const map2D_i &distributionMap);
};

#endif // GRASSGENERATOR_H
