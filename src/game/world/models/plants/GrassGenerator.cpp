#include "game/world/models/plants/GrassGenerator.h"

GrassGenerator::GrassGenerator()
  :
    PlantGenerator()
{
  models.reserve(8);
  models.emplace_back("grass/grass1/grass1.obj", false);
  models.emplace_back("grass/grass2/grass2.obj", false);
  models.emplace_back("grass/grass3/grass3.obj", false);
  models.emplace_back("grass/grass4/grass4.obj", false);
  models.emplace_back("grass/grass5/grass5.obj", false);
  models.emplace_back("grass/grass6/grass6.obj", false);

  lowPolyModels.reserve(8);
  lowPolyModels.emplace_back("grass/grass1LP/grass1LP.obj", true);
  lowPolyModels.emplace_back("grass/grass2LP/grass2LP.obj", true);
  lowPolyModels.emplace_back("grass/grass3LP/grass3LP.obj", true);
  lowPolyModels.emplace_back("grass/grass4LP/grass4LP.obj", true);
  lowPolyModels.emplace_back("grass/grass5LP/grass5LP.obj", true);
  lowPolyModels.emplace_back("grass/grass6LP/grass6LP.obj", true);

  assert(lowPolyModels.size() == models.size());
}

void GrassGenerator::setup(const map2D_f &landMap, const map2D_f &hillMap, const map2D_i &distributionMap)
{
  setupModelChunks();
  setupMatrices(landMap, hillMap, distributionMap);
}

void GrassGenerator::setupMatrices(const map2D_f &landMap, const map2D_f &hillMap, const map2D_i &distributionMap)
{
  map2D_mat4 matricesStorage = substituteMatricesStorage();
  std::uniform_real_distribution<float> modelSizeDistribution(MIN_SCALE, MAX_SCALE);

  size_t numberOfModels = models.size();
  std::vector<unsigned int> instanceOffsetsVector(numberOfModels);
  std::vector<unsigned int> numInstancesVector(numberOfModels);
  for (unsigned int i = 0; i < numberOfModels; i++)
    {
      instanceOffsetsVector.emplace_back(0);
      numInstancesVector.emplace_back(0);
    }

  unsigned int matrixCounter = 0, chunkCounter = 0;
  for (unsigned int startY = 0; startY < WORLD_HEIGHT; startY += CHUNK_SIZE)
    {
      for (unsigned int startX = 0; startX < WORLD_WIDTH; startX += CHUNK_SIZE)
        {
          chunks.at(chunkCounter).setInstanceOffsetsVector(instanceOffsetsVector);
          for (unsigned int y = startY; y < startY + CHUNK_SIZE; y++)
            {
              for (unsigned int x = startX; x < startX + CHUNK_SIZE; x++)
                {
                  if ( (landMap[y][x] == 0 && landMap[y+1][x+1] == 0 && landMap[y+1][x] == 0 && landMap[y][x+1] == 0) &&
                      !(hillMap[y][x] != 0 || hillMap[y+1][x+1] != 0 || hillMap[y+1][x] != 0 || hillMap[y][x+1] != 0) &&
                       rand() % (MODELS_DISTRIBUTION_FREQUENCY / 2 - 1) == 0 &&
                       distributionMap[y][x] > MODELS_DISTRIBUTION_FREQUENCY / 2)
                    {
                      glm::mat4 model;
                      glm::vec3 translateVector(-HALF_WORLD_WIDTH_F + x + 0.5f, 0.0f, -HALF_WORLD_HEIGHT_F + y + 0.5f);
                      model = glm::translate(model, translateVector);
                      model = glm::rotate(model, glm::radians((float)(rand() * WORLD_WIDTH + x * 5)), glm::vec3(0.0f, 1.0f, 0.0f));
                      glm::vec3 scaleVector(modelSizeDistribution(randomizer), modelSizeDistribution(randomizer), modelSizeDistribution(randomizer));
                      model = glm::scale(model, scaleVector);

                      size_t currentModelIndex = matrixCounter % matricesStorage.size();
                      matricesStorage[currentModelIndex].emplace_back(std::move(model));
                      ++numInstancesVector[currentModelIndex];
                      ++instanceOffsetsVector[currentModelIndex];
                      ++matrixCounter;
                    }
                }
            }
          chunks.at(chunkCounter).setNumInstancesVector(numInstancesVector);
          for (unsigned int i = 0; i < numInstancesVector.size(); i++)
            numInstancesVector[i] = 0;
          ++chunkCounter;
        }
    }
  loadMatrices(matricesStorage);
}
