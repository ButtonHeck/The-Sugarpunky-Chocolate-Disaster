#include "game/world/models/plants/PlantGenerator.h"

PlantGenerator::PlantGenerator()
{
  static bool randomizerInitialized = false;
  if (!randomizerInitialized)
    {
      randomizer.seed(std::chrono::system_clock::now().time_since_epoch().count());
      randomizerInitialized = true;
    }
}

PlantGenerator::~PlantGenerator()
{
  for (size_t modelIndex = 0; modelIndex < matrices.size(); ++modelIndex)
    delete[] matrices[modelIndex];
  for (Model& model : models)
    model.cleanup();
  for (Model& model : lowPolyModels)
    model.cleanup();
}

void PlantGenerator::setupModelChunks()
{
  chunks.clear();
  for (unsigned int y = 0; y < WORLD_HEIGHT; y += CHUNK_SIZE)
    {
      for (unsigned int x = 0; x < WORLD_WIDTH; x += CHUNK_SIZE)
        chunks.emplace_back(x, x + CHUNK_SIZE, y, y + CHUNK_SIZE);
    }
}

void PlantGenerator::serialize(std::ofstream &output)
{
  for (unsigned int chunk = 0; chunk < chunks.size(); chunk++)
    {
      for (unsigned int modelIndex = 0; modelIndex < chunks[chunk].getNumInstancesVector().size(); )
        {
          if (chunks[chunk].getNumInstances(modelIndex) == 0)
            {
              unsigned int zeroesInRow = 0;
              while (modelIndex < chunks[chunk].getNumInstancesVector().size() && chunks[chunk].getNumInstances(modelIndex) == 0)
                {
                  zeroesInRow++;
                  modelIndex++;
                }
              output << 0 << " " << zeroesInRow << " ";
            }
          else
            {
              output << chunks[chunk].getNumInstances(modelIndex) << " ";
              modelIndex++;
            }
        }

      for (unsigned int modelIndex = 0; modelIndex < chunks[chunk].getInstanceOffsetVector().size(); )
        {
          if (chunks[chunk].getInstanceOffset(modelIndex) == 0)
            {
              unsigned int zeroesInRow = 0;
              while(modelIndex < chunks[chunk].getInstanceOffsetVector().size() && chunks[chunk].getInstanceOffset(modelIndex) == 0)
                {
                  zeroesInRow++;
                  modelIndex++;
                }
              output << 0 << " " << zeroesInRow << " ";
            }
          else
            {
              output << chunks[chunk].getInstanceOffset(modelIndex) << " ";
              modelIndex++;
            }
        }
    }

  for (unsigned int modelIndex = 0; modelIndex < matrices.size(); modelIndex++)
    {
      output << numPlants[modelIndex] << " ";
      for (unsigned int instanceIndex = 0; instanceIndex < numPlants[modelIndex]; instanceIndex++)
        {
          float* matrixValues = (float*)glm::value_ptr(matrices[modelIndex][instanceIndex]);
          for (unsigned int e = 0; e < 16; ++e)
            output << std::setprecision(4) << matrixValues[e] << " ";
        }
    }
}

void PlantGenerator::deserialize(std::ifstream &input)
{
  for (unsigned int chunk = 0; chunk < chunks.size(); chunk++)
    {
      for (unsigned int modelIndex = 0; modelIndex < chunks[chunk].getNumInstancesVector().size(); )
        {
          unsigned int numInstances;
          input >> numInstances;
          if (numInstances == 0)
            {
              unsigned int zeroesInRow;
              input >> zeroesInRow;
              for (unsigned int neighbourModelIndex = modelIndex; neighbourModelIndex < modelIndex + zeroesInRow; neighbourModelIndex++)
                chunks[chunk].setNumInstances(neighbourModelIndex, 0);
              modelIndex += zeroesInRow;
            }
          else
            {
              chunks[chunk].setNumInstances(modelIndex, numInstances);
              modelIndex++;
            }
        }

      for (unsigned int modelIndex = 0; modelIndex < chunks[chunk].getInstanceOffsetVector().size(); )
        {
          unsigned int offset;
          input >> offset;
          if (offset == 0)
            {
              unsigned int zeroesInRow;
              input >> zeroesInRow;
              for (unsigned int neighbourModelIndex = modelIndex; neighbourModelIndex < modelIndex + zeroesInRow; neighbourModelIndex++)
                chunks[chunk].setInstanceOffset(neighbourModelIndex, 0);
              modelIndex += zeroesInRow;
            }
          else
            {
              chunks[chunk].setInstanceOffset(modelIndex, offset);
              modelIndex++;
            }
        }
    }

  map2D_mat4 newMatrices;
  for (unsigned int modelIndex = 0; modelIndex < matrices.size(); modelIndex++)
    {
      unsigned int numPlantsForCurrentModel = 0;
      input >> numPlantsForCurrentModel;
      newMatrices.emplace_back(std::vector<glm::mat4>(numPlantsForCurrentModel));
      for (unsigned int instanceIndex = 0; instanceIndex < numPlantsForCurrentModel; instanceIndex++)
        {
          glm::mat4 model;
          float* modelData = (float*)glm::value_ptr(model);
          for (unsigned int e = 0; e < 16; e++)
            input >> modelData[e];
          newMatrices[modelIndex][instanceIndex] = std::move(model);
        }
    }
  loadMatrices(newMatrices);
}

void PlantGenerator::loadMatrices(const map2D_mat4 &newMatrices)
{
  numPlants.reset(new unsigned int[newMatrices.size()]);
  for (unsigned int modelIndex = 0; modelIndex < matrices.size(); modelIndex++)
    delete[] matrices[modelIndex];
  matrices.clear();

  for (unsigned int modelIndex = 0; modelIndex < newMatrices.size(); modelIndex++)
    {
      matrices.emplace_back(new glm::mat4[newMatrices[modelIndex].size()]);
      for (unsigned int instanceIndex = 0; instanceIndex < newMatrices[modelIndex].size(); instanceIndex++)
        matrices[modelIndex][instanceIndex] = newMatrices[modelIndex][instanceIndex];
      numPlants[modelIndex] = newMatrices[modelIndex].size();
    }

  for (unsigned int modelIndex = 0; modelIndex < models.size(); modelIndex++)
    models[modelIndex].loadModelInstances(matrices[modelIndex], numPlants[modelIndex]);
  for (unsigned int modelIndex = 0; modelIndex < lowPolyModels.size(); modelIndex++)
    lowPolyModels[modelIndex].loadModelInstances(matrices[modelIndex], numPlants[modelIndex]);
}

map2D_mat4 PlantGenerator::substituteMatricesStorage()
{
  map2D_mat4 newMatrices;
  for (unsigned int modelIndex = 0; modelIndex < models.size(); modelIndex++)
    {
      newMatrices.emplace_back(std::vector<glm::mat4>());
      if (!matrices.empty())
        delete[] matrices[modelIndex];
    }
  matrices.clear();
  return newMatrices;
}
