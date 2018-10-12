#ifndef LANDPLANTSGENERATOR_H
#define LANDPLANTSGENERATOR_H
#include "generator/PlantGenerator.h"

class LandPlantsGenerator : public PlantGenerator
{
public:
  LandPlantsGenerator();
  void setup(std::vector<std::vector<float>>& baseMap, std::vector<std::vector<float>>& hillMap);
private:
  void setupMatrices(std::vector<std::vector<float>>& baseMap, std::vector<std::vector<float>>& hillMap);
};

#endif // LANDPLANTSGENERATOR_H
