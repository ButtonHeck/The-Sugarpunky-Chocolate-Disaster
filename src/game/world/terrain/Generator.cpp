/*
 * Copyright 2019 Ilya Malgin
 * Generator.cpp
 * This file is part of The Sugarpunky Chocolate Disaster project
 *
 * The Sugarpunky Chocolate Disaster project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The Sugarpunky Chocolate Disaster project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * See <http://www.gnu.org/licenses/>
 *
 * Purpose: contains definition for Generator class
 * @version 0.1.0
 */

#include "Generator"

#include <iomanip>
#include <fstream>

/**
* @brief plain ctor. Initializes buffer collection (vao+vbo+ebo), map, reserves enough capacity for tiles storage
*/
Generator::Generator() noexcept
  :
    basicGLBuffers(VAO | VBO | EBO)
{
 initializeMap(map);
 tiles.reserve(NUM_TILES);
}

const map2D_f &Generator::getMap() const noexcept
{
  return map;
}

/**
* @brief saves map data to file
* @param output file stream to write data to
* @param setPrecision indicator of precision mode serialization
* @param precision number of digits (accuracy level) which would be written to file
*/
void Generator::serialize(std::ofstream &output, bool setPrecision, unsigned int precision)
{
  for (unsigned int row = 0; row < map.size(); row++)
    {
      for (unsigned int column = 0; column < map[row].size(); )
        {
		  /**
		  * there might be cases when there are same values in a row in a map,
		  * so it is better to write only this value and how many times in a row it appears
		  */
          if (map[row][column] == 0.0f)
            serializeRepeatValues(output, 0.0f, row, column);
          else if(map[row][column] == TILE_NO_RENDER_VALUE)
            serializeRepeatValues(output, TILE_NO_RENDER_VALUE, row, column);
          else if (map[row][column] == WATER_LEVEL)
            serializeRepeatValues(output, WATER_LEVEL, row, column);
          else
            {
              if (setPrecision)
                output << std::setprecision(precision);
              output << map[row][column] << " ";
              column++;
            }
        }
    }
}

/**
* @brief loads map data from file
* @param input file stream to read data from
*/
void Generator::deserialize(std::ifstream &input)
{
  for (unsigned int row = 0; row < map.size(); row++)
    {
      for (unsigned int column = 0; column < map[row].size(); )
        {
          float value;
          input >> value;
		  /**
		  * there might be cases when saved map contains same values in a row,
		  * so read them as a pair consisting of a value and number of its repetitions in a row
		  */
          if (value == 0.0f)
            deserializeRepeatValues(input, 0.0f, row, column);
          else if (value == TILE_NO_RENDER_VALUE)
            deserializeRepeatValues(input, TILE_NO_RENDER_VALUE, row, column);
          else if (value == WATER_LEVEL)
            deserializeRepeatValues(input, WATER_LEVEL, row, column);
          else
            {
              map[row][column] = value;
              column++;
            }
        }
    }
}

/**
* @brief utility function that makes value transitions at neighbouring coordinates smoother
* @param selfWeight percentage value defining how much impact original height value has on final result
* @param sideNeighbourWeight percentage value defining how much impact left/right/up/down neighbour height values have on final result
* @param diagonalNeighbourWeight percentage value defining how much impact diagonal neighbours height values have on final result
* @note it is programmer's responsibility to make sure that the following equation holds true: 
*	1*SelfW + 4*SideW + 4*DiagW == 1.0.
*/
void Generator::smoothMapAdjacentHeights(float selfWeight, float sideNeighbourWeight, float diagonalNeighbourWeight)
{
  //another boilerplate map is needed to prevent feedback during processing
  map2D_f mapSmoothed;
  Generator::initializeMap(mapSmoothed);
  for (unsigned int y = 1; y < WORLD_HEIGHT; y++)
    {
      for (unsigned int x = 1; x < WORLD_WIDTH; x++)
        {
          if (map[y][x] == 0)
            continue;
          float smoothedHeight =
                map[y][x] * selfWeight
              + map[y-1][x] * sideNeighbourWeight
              + map[y+1][x] * sideNeighbourWeight
              + map[y][x-1] * sideNeighbourWeight
              + map[y][x+1] * sideNeighbourWeight
              + map[y-1][x-1] * diagonalNeighbourWeight
              + map[y-1][x+1] * diagonalNeighbourWeight
              + map[y+1][x-1] * diagonalNeighbourWeight
              + map[y+1][x+1] * diagonalNeighbourWeight;
          mapSmoothed[y][x] = smoothedHeight;
        }
    }
  map.assign(mapSmoothed.begin(), mapSmoothed.end());
}

/**
* @brief utility function that creates map of normal vectors at each coordinate of the source map
* @param normalMap map of normal vectors to be filled
*/
void Generator::createNormalMap(map2D_vec3 &normalMap)
{
  using glm::vec3;

  //first initialize and reserve enough capacity for normal map
  normalMap.clear();
  normalMap.reserve(WORLD_HEIGHT + 1);
  for (size_t row = 0; row < WORLD_HEIGHT + 1; row++)
    {
      vec3 defaultNormal(0.0f, 1.0f, 0.0f);
      std::vector<vec3> defaultNormalsVec(WORLD_WIDTH + 1, defaultNormal);
      normalMap.emplace_back(defaultNormalsVec);
    }

  //create normals for each coordinate of the source map
  for (unsigned int y = 1; y < map.size() - 1; y++)
    {
      for (unsigned int x = 1; x < map[0].size() - 1; x++)
        {
          vec3 n0 = glm::normalize(vec3(map[y][x-1] - map[y][x], 1, map[y-1][x] - map[y][x]));
          vec3 n3 = glm::normalize(vec3(map[y][x] - map[y][x+1], 1, map[y-1][x+1] - map[y][x+1]));
          vec3 n6 = glm::normalize(vec3(map[y+1][x-1] - map[y+1][x], 1, map[y][x] - map[y+1][x]));
          vec3 n1 = glm::normalize(vec3(map[y-1][x] - map[y-1][x+1], 1, map[y-1][x] - map[y][x]));
          vec3 n4 = glm::normalize(vec3(map[y][x] - map[y][x+1], 1, map[y][x] - map[y+1][x]));
          vec3 n9 = glm::normalize(vec3(map[y][x-1] - map[y][x], 1, map[y][x-1] - map[y+1][x-1]));
          vec3 averagedNormal = glm::normalize(n0 + n1 + n3 + n4 + n6 + n9);
          normalMap[y][x] = averagedNormal;
        }
    }

  /* 
   * make sure that we do not have a default normal where it should not be (0,1,0)
   * in this case just assign an approximation of three already calculated nearby normals
   * case 1: incorrect normal on the upper-left edge of the surface with no adjacent surfaces
   * case 2: incorrect normal on the bottom-right edge of the surface with no adjacent surfaces
   */
  for (unsigned int y = 1; y < map.size() - 1; y++)
    {
      for (unsigned int x = 1; x < map[0].size() - 1; x++)
        {
          //case 1
          if (normalMap[y][x].y == 1.0f)
            normalMap[y][x] = glm::normalize(normalMap[y][x+1] + normalMap[y+1][x+1] + normalMap[y+1][x]);
          //case 2 (if we still have (0,1,0) - that must be the normal on a bottom-right edge)
          if (normalMap[y][x].y == 1.0f)
            normalMap[y][x] = glm::normalize(normalMap[y][x-1] + normalMap[y-1][x-1] + normalMap[y-1][x]);
        }
    }
}

/**
* @brief helper serialization function that records series of same tokens in a row as a pair value/count
* @param output file stream to write data to
* @param value value that remains the same in a row
* @param row row index of the map
* @param column column index of the map
*/
template<typename T>
void Generator::serializeRepeatValues(std::ofstream &output,
                                      T value,
                                      const unsigned int& row,
                                      unsigned int& column)
{
  unsigned int repeatValuesInRow = 0;
  while (column < map[row].size() && map[row][column] == value)
    {
      repeatValuesInRow++;
      column++;
    }
  output << value << " " << repeatValuesInRow << " ";
}

/**
* @brief helper deserialization function that unpacks a pair value/count as a series of same tokens
* @param input file stream to read data from
* @param value value that remains the same in a row
* @param row row index of the map
* @param column column index of the map
*/
template<typename T>
void Generator::deserializeRepeatValues(std::ifstream &input,
                                        T value,
                                        const unsigned int &row,
                                        unsigned int &column)
{
  unsigned int repeatValuesInRow;
  input >> repeatValuesInRow;
  for (unsigned int i = column; i < column + repeatValuesInRow; i++)
    map[row][i] = value;
  column += repeatValuesInRow;
}
