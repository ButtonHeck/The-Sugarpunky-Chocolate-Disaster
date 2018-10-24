#ifndef LANDPLANTSGENERATOR_H
#define LANDPLANTSGENERATOR_H
#include "game/world/plants/PlantGenerator.h"

class LandPlantsGenerator : public PlantGenerator
{
public:
  LandPlantsGenerator();
  void setup(std::vector<std::vector<float>>& baseMap,
             std::vector<std::vector<float>>& hillMap,
             std::vector<std::vector<int>>& distributionMap);
private:
  void setupMatrices(std::vector<std::vector<float>>& baseMap, std::vector<std::vector<float>>& hillMap, std::vector<std::vector<int> > &distributionMap);
};

#endif // LANDPLANTSGENERATOR_H
