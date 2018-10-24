#ifndef GRASSGENERATOR_H
#define GRASSGENERATOR_H
#include "game/world/plants/PlantGenerator.h"

class GrassGenerator : public PlantGenerator
{
public:
  GrassGenerator();
  void setup(std::vector<std::vector<float>>& baseMap,
             std::vector<std::vector<float>>& hillMap,
             std::vector<std::vector<int>>& distributionMap);
private:
  void setupMatrices(std::vector<std::vector<float>>& baseMap, std::vector<std::vector<float>>& hillMap, std::vector<std::vector<int> > &distributionMap);
};

#endif // GRASSGENERATOR_H
