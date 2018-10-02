#include "game/SaveLoadManager.h"

SaveLoadManager::SaveLoadManager(std::shared_ptr<BaseMapGenerator>& baseGenerator,
                                 std::shared_ptr<HillsMapGenerator>& hillGenerator,
                                 std::shared_ptr<WaterMapGenerator>& waterGenerator,
                                 std::shared_ptr<BuildableMapGenerator>& buildableGenerator,
                                 std::shared_ptr<PlantGenerator>& treeGenerator,
                                 Camera& camera)
  :
    baseGenerator(baseGenerator),
    hillGenerator(hillGenerator),
    waterGenerator(waterGenerator),
    buildableGenerator(buildableGenerator),
    plantGenerator(treeGenerator),
    camera(camera),
    baseMap(baseGenerator->getMap()),
    hillMap(hillGenerator->getMap()),
    waterMap(waterGenerator->getMap()),
    chunkMap(baseGenerator->getChunkMap())
{

}

bool SaveLoadManager::saveToFile(const std::string &filename)
{
  std::ofstream output(filename);
  if (!output)
    {
      std::cerr << "Could not open file: " << filename << std::endl;
      return false;
    }
  output << WORLD_WIDTH << " " << WORLD_HEIGHT << " ";
  for (std::vector<float>& row : baseMap)
    {
      for (float& value : row)
        {
          output << value << " ";
        }
    }
  for (std::vector<float>& row : chunkMap)
    {
      for (unsigned int i = 0; i < row.size(); i++)
        output << 0 << " ";
    }
  for (std::vector<float>& row : hillMap)
    {
      for (float& value : row)
        {
          output << value << " ";
        }
    }
  for (std::vector<float>& row : waterMap)
    {
      for (float& value : row)
        {
          output << value << " ";
        }
    }
  plantGenerator->serialize(output);
  output << camera.getPosition().x << " ";
  output << camera.getPosition().y << " ";
  output << camera.getPosition().z << " ";
  output << camera.getPitch() << " ";
  output << camera.getYaw() << " ";
  output.close();
  return true;
}

bool SaveLoadManager::loadFromFile(const std::string &filename)
{
  std::ifstream input(filename);
  if (!input)
    {
      std::cerr << "Could not open file: " << filename;
      return false;
    }
  int tiles_width, tiles_height;
  input >> tiles_width >> tiles_height;
  for (std::vector<float>& row : baseMap)
    {
      for (float& value : row)
        {
          input >> value;
        }
    }
  for (std::vector<float>& row : chunkMap)
    {
      for (float& value : row)
        {
          input >> value;
        }
    }
  for (std::vector<float>& row : hillMap)
    {
      for (float& value : row)
        {
          input >> value;
        }
    }
  for (std::vector<float>& row : waterMap)
    {
      for (float& value : row)
        {
          input >> value;
        }
    }

  std::vector<ModelChunk>& plainPlantsModelChunks = plantGenerator->getPlainPlantsModelChunks();
  for (unsigned int chunk = 0; chunk < plainPlantsModelChunks.size(); chunk++)
    {
      for (unsigned int i = 0; i < plainPlantsModelChunks[chunk].getNumInstancesVector().size(); i++)
        {
          unsigned int numInstances;
          input >> numInstances;
          plainPlantsModelChunks[chunk].setNumInstances(i, numInstances);
        }
      for (unsigned int i = 0; i < plainPlantsModelChunks[chunk].getInstanceOffsetVector().size(); i++)
        {
          unsigned int offset;
          input >> offset;
          plainPlantsModelChunks[chunk].setInstanceOffset(i, offset);
        }
    }
  std::vector<ModelChunk>& hillTreeModelChunks = plantGenerator->getHillTreeModelChunks();
  for (unsigned int chunk = 0; chunk < hillTreeModelChunks.size(); chunk++)
    {
      for (unsigned int i = 0; i < hillTreeModelChunks[chunk].getNumInstancesVector().size(); i++)
        {
          unsigned int numInstances;
          input >> numInstances;
          hillTreeModelChunks[chunk].setNumInstances(i, numInstances);
        }
      for (unsigned int i = 0; i < hillTreeModelChunks[chunk].getInstanceOffsetVector().size(); i++)
        {
          unsigned int offset;
          input >> offset;
          hillTreeModelChunks[chunk].setInstanceOffset(i, offset);
        }
    }

  std::vector<glm::mat4*> plainPlantsMatrices;
  unsigned int numAllTrees[plantGenerator->getPlainPlantsMatrices().size()];
  for (unsigned int i = 0; i < plantGenerator->getPlainPlantsMatrices().size(); i++)
    {
      unsigned int numTrees = 0;
      input >> numTrees;
      numAllTrees[i] = numTrees;
      plainPlantsMatrices.emplace_back(new glm::mat4[numTrees]);
      for (unsigned int t = 0; t < numTrees; t++)
        {
          glm::mat4 model;
          float* modelData = (float*)glm::value_ptr(model);
          for (unsigned int e = 0; e < 16; e++)
            {
              input >> modelData[e];
            }
          plainPlantsMatrices[i][t] = std::move(model);
        }
    }
  std::vector<glm::mat4*> hillTreesMatrices;
  unsigned int numAllHillTrees[plantGenerator->getHillTreesMatrices().size()];
  for (unsigned int i = 0; i < plantGenerator->getHillTreesMatrices().size(); i++)
    {
      unsigned int numHillTrees = 0;
      input >> numHillTrees;
      numAllHillTrees[i] = numHillTrees;
      hillTreesMatrices.emplace_back(new glm::mat4[numHillTrees]);
      for (unsigned int m = 0; m < numHillTrees; m++)
        {
          glm::mat4 model;
          float* modelData = (float*)glm::value_ptr(model);
          for (unsigned int e = 0; e < 16; e++)
            {
              input >> modelData[e];
            }
          hillTreesMatrices[i][m] = std::move(model);
        }
    }
  plantGenerator->updatePlainModels(plainPlantsMatrices, numAllTrees);
  plantGenerator->updateHillModels(hillTreesMatrices, numAllHillTrees);
  for (unsigned int i = 0; i < plantGenerator->getPlainPlantsMatrices().size(); i++)
    {
      delete[] plainPlantsMatrices[i];
    }
  for (unsigned int i = 0; i < plantGenerator->getHillTreesMatrices().size(); i++)
    {
      delete[] hillTreesMatrices[i];
    }

  hillGenerator->createTiles();
  hillGenerator->fillBufferData();
  baseGenerator->getSquareTiles().clear();
  baseGenerator->getCellTiles().clear();
  baseGenerator->prepareMap();
  baseGenerator->fillShoreBufferData();
  baseGenerator->fillSquareBufferData();
  baseGenerator->fillCellBufferData();
  waterGenerator->postPrepareMap();
  waterGenerator->fillBufferData();

  float camPosX, camPosY, camPosZ, pitch, yaw;
  input >> camPosX >> camPosY >> camPosZ >> pitch >> yaw;
  camera.setPosition(camPosX, camPosY, camPosZ);
  camera.setPitch(pitch);
  camera.setYaw(yaw);
  camera.updateVectors();
  return true;
}
