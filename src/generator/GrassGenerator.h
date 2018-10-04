#ifndef GRASSGENERATOR_H
#define GRASSGENERATOR_H
#include "generator/PlantGenerator.h"

class GrassGenerator : public PlantGenerator
{
public:
  GrassGenerator();
  void setup(std::vector<std::vector<float>>& baseMap, std::vector<std::vector<float>>& hillMap);
private:
  void setupMatrices(std::vector<std::vector<float>>& baseMap, std::vector<std::vector<float>>& hillMap);
};

#endif // GRASSGENERATOR_H
