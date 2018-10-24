#ifndef HILLTREESGENERATOR_H
#define HILLTREESGENERATOR_H
#include "game/world/plants/PlantGenerator.h"

class HillTreesGenerator : public PlantGenerator
{
public:
  HillTreesGenerator();
  void setup(std::vector<std::vector<float>>& hillMap,
             std::vector<std::vector<int>>& distributionMap);
private:
  void setupMatrices(std::vector<std::vector<float>>& hillMap,
                     std::vector<std::vector<int>>& distributionMap);
};

#endif // HILLTREESGENERATOR_H
