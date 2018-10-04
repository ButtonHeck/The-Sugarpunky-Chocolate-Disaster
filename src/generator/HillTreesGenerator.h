#ifndef HILLTREESGENERATOR_H
#define HILLTREESGENERATOR_H
#include "generator/PlantGenerator.h"

class HillTreesGenerator : public PlantGenerator
{
public:
  HillTreesGenerator();
  void setup(std::vector<std::vector<float>>& hillMap);
private:
  void setupMatrices(std::vector<std::vector<float>>& hillMap);
};

#endif // HILLTREESGENERATOR_H
