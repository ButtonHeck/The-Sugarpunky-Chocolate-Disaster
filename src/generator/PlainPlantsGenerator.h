#ifndef PLAINPLANTSGENERATOR_H
#define PLAINPLANTSGENERATOR_H
#include "generator/PlantGenerator.h"

class PlainPlantsGenerator : public PlantGenerator
{
public:
  PlainPlantsGenerator();
  void setup(std::vector<std::vector<float>>& baseMap, std::vector<std::vector<float>>& hillMap);
private:
  void setupMatrices(std::vector<std::vector<float>>& baseMap, std::vector<std::vector<float>>& hillMap);
};

#endif // PLAINPLANTSGENERATOR_H
