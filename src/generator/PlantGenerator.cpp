#include "generator/PlantGenerator.h"

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
  delete[] numPlants;
  for (size_t i = 0; i < matrices.size(); ++i)
    delete[] matrices[i];
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

std::vector<Model> &PlantGenerator::getModels()
{
  return models;
}

std::vector<ModelChunk> &PlantGenerator::getChunks()
{
  return chunks;
}

void PlantGenerator::serialize(std::ofstream &output)
{
  for (unsigned int chunk = 0; chunk < chunks.size(); chunk++)
    {
      for (unsigned int i = 0; i < chunks[chunk].getNumInstancesVector().size(); i++)
        output << chunks[chunk].getNumInstances(i) << " ";
      for (unsigned int i = 0; i < chunks[chunk].getInstanceOffsetVector().size(); i++)
        output << chunks[chunk].getInstanceOffset(i) << " ";
    }

  for (unsigned int i = 0; i < matrices.size(); i++)
    {
      output << numPlants[i] << " ";
      for (unsigned int m = 0; m < numPlants[i]; m++)
        {
          float* matrixValues = (float*)glm::value_ptr(matrices[i][m]);
          for (unsigned int e = 0; e < 16; ++e)
            output << matrixValues[e] << " ";
        }
    }
}

void PlantGenerator::deserialize(std::ifstream &input)
{
  for (unsigned int chunk = 0; chunk < chunks.size(); chunk++)
    {
      for (unsigned int i = 0; i < chunks[chunk].getNumInstancesVector().size(); i++)
        {
          unsigned int numInstances;
          input >> numInstances;
          chunks[chunk].setNumInstances(i, numInstances);
        }
      for (unsigned int i = 0; i < chunks[chunk].getInstanceOffsetVector().size(); i++)
        {
          unsigned int offset;
          input >> offset;
          chunks[chunk].setInstanceOffset(i, offset);
        }
    }

  std::vector<glm::mat4*> newMatrices;
  unsigned int numAllTrees[matrices.size()];
  for (unsigned int i = 0; i < matrices.size(); i++)
    {
      unsigned int numTrees = 0;
      input >> numTrees;
      numAllTrees[i] = numTrees;
      newMatrices.emplace_back(new glm::mat4[numTrees]);
      for (unsigned int t = 0; t < numTrees; t++)
        {
          glm::mat4 model;
          float* modelData = (float*)glm::value_ptr(model);
          for (unsigned int e = 0; e < 16; e++)
            input >> modelData[e];
          newMatrices[i][t] = std::move(model);
        }
    }

  updateMatrices(newMatrices, numAllTrees);
  for (unsigned int i = 0; i < matrices.size(); i++)
    delete[] newMatrices[i];
}

void PlantGenerator::updateMatrices(std::vector<glm::mat4 *> &newMatrices, unsigned int *numAllTrees)
{
  delete[] numPlants;
  numPlants = new unsigned int[newMatrices.size()];
  for (unsigned int i = 0; i < matrices.size(); i++)
    delete[] matrices[i];
  matrices.clear();

  for (unsigned int i = 0; i < newMatrices.size(); i++)
    {
      matrices.emplace_back(new glm::mat4[numAllTrees[i]]);
      numPlants[i] = numAllTrees[i];
    }
  for (unsigned int i = 0; i < matrices.size(); i++)
    {
      for (unsigned int m = 0; m < numAllTrees[i]; m++)
        matrices[i][m] = newMatrices[i][m];
    }

  for (unsigned int i = 0; i < newMatrices.size(); i++)
    models[i].loadInstances(matrices[i], numPlants[i]);
}

std::vector<std::vector<glm::mat4> > PlantGenerator::substituteMatricesStorage()
{
  std::vector<std::vector<glm::mat4>> newMatrices;
  for (unsigned int i = 0; i < models.size(); i++)
    {
      newMatrices.emplace_back(std::vector<glm::mat4>());
      if (!matrices.empty())
        delete[] matrices[i];
    }
  matrices.clear();
  return newMatrices;
}

void PlantGenerator::loadMatrices(std::vector<std::vector<glm::mat4> > &newMatrices)
{
  if (numPlants)
    delete[] numPlants;
  numPlants = new unsigned int[models.size()];
  for (unsigned int i = 0; i < newMatrices.size(); i++)
    {
      matrices.emplace_back(new glm::mat4[newMatrices[i].size()]);
      for (unsigned int m = 0; m < newMatrices[i].size(); m++)
        matrices[i][m] = newMatrices[i][m];
      numPlants[i] = newMatrices[i].size();
    }
  for (unsigned int i = 0; i < models.size(); i++)
    models[i].loadInstances(matrices[i], numPlants[i]);
}
