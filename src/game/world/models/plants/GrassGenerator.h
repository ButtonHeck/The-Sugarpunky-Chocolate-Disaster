#ifndef GRASSGENERATOR_H
#define GRASSGENERATOR_H
#include "game/world/models/plants/PlantGenerator.h"

class GrassGenerator : public PlantGenerator
{
public:
  GrassGenerator();
  void setup(const map2D_f &landMap, const map2D_f &hillMap, const map2D_i &distributionMap);

private:
  const float MIN_SCALE = 0.189f;
  const float MAX_SCALE = 0.224f;
  void setupMatrices(const map2D_f &landMap, const map2D_f &hillMap, const map2D_i &distributionMap);
};

#endif // GRASSGENERATOR_H
