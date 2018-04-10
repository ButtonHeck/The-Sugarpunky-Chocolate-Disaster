#include "SaveLoadManager.h"

SaveLoadManager::SaveLoadManager(BaseMapGenerator &baseGenerator, HillsMapGenerator &hillGenerator, WaterMapGenerator &waterGenerator)
  :
    baseGenerator(baseGenerator),
    hillGenerator(hillGenerator),
    waterGenerator(waterGenerator),
    baseMap(baseGenerator.getMap()),
    hillMap(hillGenerator.getMap()),
    waterMap(waterGenerator.getMap()),
    chunkMap(baseGenerator.getChunkMap())
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
  output << TILES_WIDTH << " " << TILES_HEIGHT << " ";
  for (std::vector<float>& row : baseMap)
    {
      for (float& value : row)
        {
          output << value << " ";
        }
    }
  for (std::vector<float>& row : chunkMap)
    {
      for (float& value : row)
        {
          output << value << " ";
        }
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
  hillGenerator.createTiles(false, false);
  hillGenerator.fillBufferData();
  baseGenerator.getCellTiles().clear();
  for (unsigned int i = 0; i < NUM_BASE_TERRAIN_CHUNKS; i++)
    {
      baseGenerator.getChunkTiles(i).clear();
    }
  baseGenerator.split1x1Tiles();
  baseGenerator.createTiles(false, false);
  baseGenerator.fillBufferData();
  baseGenerator.fillChunkBufferData();
  baseGenerator.fillCellBufferData();
  waterGenerator.createTiles(true, false);
  waterGenerator.fillBufferData();
  return true;
}
