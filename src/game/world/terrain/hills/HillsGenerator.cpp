/*
 * Copyright 2019 Ilya Malgin
 * HillsGenerator.cpp
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
 * Purpose: contains definitions for HillsGenerator class and HillVertex struct
 * @version 0.1.0
 */

#include "HillsGenerator"
#include "HillsShader"

#include <chrono>

/**
* @brief plain ctor, for culled buffer pipeline need only vao+vbo+transform feedback. Initializes randomizer seed.
* @param shaders hills shader manager
* @param waterMap map of the water tiles
*/
HillsGenerator::HillsGenerator(HillsShader &shaders, const map2D_f &waterMap)
  :
    Generator(),
    culledBuffers(VAO | VBO | TFBO),
    shaders(shaders),
	maxHeight(1.0f),
    waterMap(waterMap)
{
  randomizer.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

/**
* @brief prepares hills maps and buffer collections
*/
void HillsGenerator::setup()
{
  //in case of recreation need to reinit maximum height value
  maxHeight = 1.0f;
  generateMap(12, HILL_DENSITY::HILLS_DENSE);
  generateMap(6, HILL_DENSITY::HILLS_THIN);
  smoothMapSinks();
  compressMap(0.00f, 1.33f); //compress entire range
  compressMap(0.66f * maxHeight, 2.0f); //compress top-most peaks
  removePlateaus(1.0f);
  for (unsigned int i = 0; i < 4; i++)
    {
      smoothMapAdjacentHeights(0.6f, 0.05f, 0.05f);
    }
  createTiles();
  createAuxiliaryMaps();
  fillBufferData();
}

/**
* @brief creates tiles based on map data
*/
void HillsGenerator::createTiles()
{
  //in case of recreation remove old tiles
  tiles.clear();

  for (unsigned int y = 1; y < map.size(); y++)
    {
      for (unsigned int x = 1; x < map[0].size(); x++)
        {
          if (map[y][x] == TILE_NO_RENDER_VALUE)
            continue;
          if (map[y][x] != 0 || map[y-1][x] != 0 || map[y][x-1] != 0 || map[y-1][x-1] != 0)
            {
              float ll = map[y][x-1] + HILLS_OFFSET_Y;
              float lr = map[y][x] + HILLS_OFFSET_Y;
              float ur = map[y-1][x] + HILLS_OFFSET_Y;
              float ul = map[y-1][x-1] + HILLS_OFFSET_Y;
              tiles.emplace_back(x, y, ll, lr, ur, ul);
            }
        }
    }
  tiles.shrink_to_fit();
}

/**
* @brief creates normal/tangent/bitangent maps used for normal mapping
*/
void HillsGenerator::createAuxiliaryMaps()
{
	createNormalMap(normalMap);
	createTangentMap();
	createBitangentMap();
}

/**
* @brief for each tile creates set of vertices, buffers them to GPU. Prepares buffer collections layouts and bindings
*/
void HillsGenerator::fillBufferData()
{
  const size_t VERTEX_DATA_LENGTH = tiles.size() * UNIQUE_VERTICES_PER_TILE * HillVertex::NUMBER_OF_ELEMENTS;
  const size_t INDICES_DATA_LENGTH = tiles.size() * VERTICES_PER_QUAD;
  size_t indicesBufferIndex = 0;
  std::unique_ptr<GLfloat[]> vertices(new GLfloat[VERTEX_DATA_LENGTH]);
  std::unique_ptr<GLuint[]> indices(new GLuint[INDICES_DATA_LENGTH]);

  for (unsigned int tileIndex = 0; tileIndex < tiles.size(); tileIndex++)
    {
      TerrainTile& tile = tiles[tileIndex];

	  //at some places of map we should set another order of vertices for better looking result
      bool verticesAlternativeOrder = false;
      if (tile.lowRight < tile.upperLeft || tile.upperLeft < tile.lowRight)
        verticesAlternativeOrder = true;

	  int x = tile.mapX, y = tile.mapY;

      //map hill texture to cover HILL_TILING_PER_TEXTURE_QUAD^2 tiles instead of one
      float tilingSizeReciprocal = 1.0f / HILL_TILING_PER_TEXTURE_QUAD;
      float texCoordXOffset = (x % HILL_TILING_PER_TEXTURE_QUAD) * tilingSizeReciprocal;
      float texCoordYOffset = ((WORLD_HEIGHT - y) % HILL_TILING_PER_TEXTURE_QUAD) * tilingSizeReciprocal;

	  //create set of vertices according to a tile
      HillVertex lowLeft(glm::vec3(x - 1, tile.lowLeft, y),
                         glm::vec2(texCoordXOffset, texCoordYOffset),
                         normalMap[y][x-1],
                         tangentMap[y][x-1],
                         bitangentMap[y][x-1]);
      HillVertex lowRight(glm::vec3(x, tile.lowRight, y),
                          glm::vec2(tilingSizeReciprocal + texCoordXOffset, texCoordYOffset),
                          normalMap[y][x],
                          tangentMap[y][x],
                          bitangentMap[y][x]);
      HillVertex upRight(glm::vec3(x, tile.upperRight, y - 1),
                         glm::vec2(tilingSizeReciprocal + texCoordXOffset, tilingSizeReciprocal + texCoordYOffset),
                         normalMap[y-1][x],
                         tangentMap[y-1][x],
                         bitangentMap[y-1][x]);
      HillVertex upLeft(glm::vec3(x - 1, tile.upperLeft, y - 1),
                        glm::vec2(texCoordXOffset, tilingSizeReciprocal + texCoordYOffset),
                        normalMap[y-1][x-1],
                        tangentMap[y-1][x-1],
                        bitangentMap[y-1][x-1]);

	  //buffer vertices to local storage
      int vertexBufferOffset = tileIndex * UNIQUE_VERTICES_PER_TILE * HillVertex::NUMBER_OF_ELEMENTS;
      bufferVertex(vertices.get(), vertexBufferOffset + HillVertex::NUMBER_OF_ELEMENTS * 0, lowLeft);
      bufferVertex(vertices.get(), vertexBufferOffset + HillVertex::NUMBER_OF_ELEMENTS * 1, lowRight);
      bufferVertex(vertices.get(), vertexBufferOffset + HillVertex::NUMBER_OF_ELEMENTS * 2, upRight);
      bufferVertex(vertices.get(), vertexBufferOffset + HillVertex::NUMBER_OF_ELEMENTS * 3, upLeft);

	  //buffer indices to local storage
      GLuint indicesBufferBaseVertex = tileIndex * UNIQUE_VERTICES_PER_TILE;
      indices[indicesBufferIndex++] = indicesBufferBaseVertex + (verticesAlternativeOrder ? 3 : 0);
      indices[indicesBufferIndex++] = indicesBufferBaseVertex + (verticesAlternativeOrder ? 0 : 1);
      indices[indicesBufferIndex++] = indicesBufferBaseVertex + (verticesAlternativeOrder ? 1 : 2);
      indices[indicesBufferIndex++] = indicesBufferBaseVertex + (verticesAlternativeOrder ? 1 : 2);
      indices[indicesBufferIndex++] = indicesBufferBaseVertex + (verticesAlternativeOrder ? 2 : 3);
      indices[indicesBufferIndex++] = indicesBufferBaseVertex + (verticesAlternativeOrder ? 3 : 0);
    }
  //buffer vertices and indices from local storage to GPU
  basicGLBuffers.bind(VAO | VBO | EBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * VERTEX_DATA_LENGTH, vertices.get(), GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * INDICES_DATA_LENGTH, indices.get(), GL_STATIC_DRAW);
  setupVBOAttributes();

  //prepare buffer collection used for frustum culling
  if (culledBuffers.get(VAO) != 0)
  {
	  culledBuffers.deleteBuffers();
	  culledBuffers.create(VAO | VBO | TFBO);
  }  
  culledBuffers.bind(VAO | VBO | TFBO);
  /* 
   * after transform feedback we have 1.5 times more data to buffer in GL_ARRAY_BUFFER
   * (6 indices of 4 vertices transforms into 6 vertices, 2 of them have the same attributes)
   * because of duplication of vertices that have same attributes but different indices
   */
  const GLsizeiptr CULLED_DATA_SIZE_BYTES = (GLsizeiptr)((float)VERTEX_DATA_LENGTH * 1.5f) * sizeof(GLfloat);
  glNamedBufferStorage(culledBuffers.get(VBO), CULLED_DATA_SIZE_BYTES, 0, GL_NONE);
  setupVBOAttributes();
  shaders.setupCulling();
  glTransformFeedbackBufferBase(culledBuffers.get(TFBO), 0, culledBuffers.get(VBO));
  BufferCollection::bindZero(VAO | VBO | EBO);
}

/**
* @brief generates unprocessed data for hills on the world map
* @param cycles number of times hills fattening would take place
* @param density empiric value defining randomizer's "hit-ratio" during generating process
*/
void HillsGenerator::generateMap(int cycles, float density)
{
  generateKernel(cycles, density);
  fattenKernel(cycles);
}

/**
* @brief generates kernel points of the hills which are to be fattened later
* @param cycles number of times hills fattening would take place, used to limit kernel generation nearby water
* @param density empiric value defining randomizer's "hit-ratio" during generating process
*/
void HillsGenerator::generateKernel(int cycles, float density)
{
  for (int y = 1; y < WORLD_HEIGHT - 1; y++)
    {
      for (int x = 1; x < WORLD_WIDTH - 1; x++)
        {
          if (rand() % (int)density == 0 && !hasWaterNearby(x, y, cycles + 3))
            map[y][x] += 1.0f;
        }
    }
}

/**
* @brief builds up hills for a given number of cycles around previously generated kernel points
* @param cycles number of times hills fattening would take place
*/
void HillsGenerator::fattenKernel(int cycles)
{
  const float CYCLE_FATTENING_DAMPING_FACTOR = 0.05f;
  const float MIN_FATTENING_HEIGHT = 0.3f;
  const float MAX_FATTENING_HEIGHT = 0.8f;

  std::uniform_real_distribution<float> heightDistribution(MIN_FATTENING_HEIGHT, MAX_FATTENING_HEIGHT);
  for (int cycle = 1; cycle <= cycles; cycle++)
    {
      for (int startY = cycle; startY < WORLD_HEIGHT - cycle; startY++)
        {
          for (int startX = cycle; startX < WORLD_WIDTH - cycle; startX++)
            {
              if (startY >= WORLD_HEIGHT)
                break;
              if (map[startY][startX] != 0 && (rand() % (cycle+1) == cycle))
                {
                  int left = (startX-cycle <= cycle ? cycle : startX-cycle);
                  int right = (startX+cycle >= WORLD_WIDTH-cycle-1 ? WORLD_WIDTH-cycle-1 : startX+cycle);
                  int top = (startY-cycle <= cycle ? cycle : startY-cycle);
                  int bottom = (startY+cycle >= WORLD_HEIGHT-cycle-1 ? WORLD_HEIGHT-cycle-1 : startY+cycle);
                  for (int y = top; y <= bottom; y++)
                    {
                      for (int x = left; x <= right; x++)
                        {
                          if (rand() % (cycle + 2) > 1)
                            {
                              //shortening the fattening borders to prevent hills generating over the shore and water
                              if (hasWaterNearby(x, y, 3 + SHORE_SMOOTH_CYCLES))
                                {
                                  --bottom;
                                  --right;
                                  continue;
                                }
                              map[y][x] += 1.0f - CYCLE_FATTENING_DAMPING_FACTOR * cycle + (heightDistribution(randomizer));
                              if (map[y][x] > maxHeight)
                                maxHeight = map[y][x];
                            }
                        }
                    }
                  //comment these two lines to see the Chocolate Grand Canyon
                  startX += cycle;
                  startY += cycle;
                }
            }
        }
        removeOrphanHills();
    }
}

/**
* @brief fills given buffer with vertex data
* @param buffer buffer to be filled
* @param offset index offset to the buffer
* @param vertex vertex to be buffered
*/
void HillsGenerator::bufferVertex(GLfloat* buffer, int offset, HillVertex vertex) noexcept
{
  buffer[offset] =   vertex.position.x;
  buffer[offset+1] = vertex.position.y;
  buffer[offset+2] = vertex.position.z;
  buffer[offset+3] = vertex.texCoords.x;
  buffer[offset+4] = vertex.texCoords.y;
  buffer[offset+5] = vertex.normal.x;
  buffer[offset+6] = vertex.normal.y;
  buffer[offset+7] = vertex.normal.z;
  buffer[offset+8] = vertex.tangent.x;
  buffer[offset+9] = vertex.tangent.y;
  buffer[offset+10] = vertex.tangent.z;
  buffer[offset+11] = vertex.bitangent.x;
  buffer[offset+12] = vertex.bitangent.y;
  buffer[offset+13] = vertex.bitangent.z;
}

/**
* @brief setup vertex buffer object attributes and pointers
*/
void HillsGenerator::setupVBOAttributes() noexcept
{
  const size_t SIZE_OF_HILL_VERTEX =  HillVertex::NUMBER_OF_ELEMENTS * sizeof(GLfloat);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, SIZE_OF_HILL_VERTEX, 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, SIZE_OF_HILL_VERTEX, (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, SIZE_OF_HILL_VERTEX, (void*)(5 * sizeof(GLfloat)));
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, SIZE_OF_HILL_VERTEX, (void*)(8 * sizeof(GLfloat)));
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, SIZE_OF_HILL_VERTEX, (void*)(11 * sizeof(GLfloat)));
}

/**
* @brief checks for any water tiles nearby given coordinates at given radius
* @param centerX X coordinate of the point around which test would run
* @param centerY Y coordinate of the point around which test would run
* @param radius defines how far from the source coordinate test would run
*/
bool HillsGenerator::hasWaterNearby(int centerX, int centerY, int radius)
{
  int xLeft = (centerX-radius <= 0 ? 0 : centerX-radius);
  int xRight = (centerX+radius >= WORLD_WIDTH ? WORLD_WIDTH : centerX+radius);
  int yTop = (centerY-radius <= 0 ? 0 : centerY-radius);
  int yBottom = (centerY+radius >= WORLD_HEIGHT ? WORLD_HEIGHT : centerY+radius);
  for (int x = xLeft; x <= xRight; x++)
    {
      for (int y = yTop; y <= yBottom; y++)
        {
          if (waterMap[y][x] != 0)
            return true;
        }
    }
  return false;
}

/**
* @brief applies basic value compression to the map
* @param thresholdAbsValue absolute minimum height value from which compression will be applied
* @param ratio compression ratio
*/
void HillsGenerator::compressMap(float thresholdAbsValue, float ratio)
{
  for (auto& row : map)
    for (auto& height : row)
      {
        if (height < thresholdAbsValue)
          continue;
        height = thresholdAbsValue + (height - thresholdAbsValue) / ratio;
      }

  //explicitly update current maximum height since it is obviously have changed after compression
  updateMaxHeight();
}

/**
* @brief walks through the map to update maximum height
*/
void HillsGenerator::updateMaxHeight()
{
  float newMaxHeight = 0.0f;
  for (int y = 1; y < WORLD_HEIGHT - 1; y++)
    {
      for (int x = 1; x < WORLD_WIDTH - 1; x++)
        {
          if (map[y][x] > newMaxHeight)
            newMaxHeight = map[y][x];
        }
    }
  maxHeight = newMaxHeight;
}

/**
* @brief remove low flat regions on the map. As a side-effect it makes hills look more natural
* @param plateauHeight reference height value below which a region supposed to be treated as 'low'
*/
void HillsGenerator::removePlateaus(float plateauHeight)
{
  unsigned int yTop, yBottom, xLeft, xRight;
  for (unsigned int y = 1; y < WORLD_HEIGHT - 1; y++)
    {
      for (unsigned int x = 1; x < WORLD_WIDTH - 1; x++)
        {
          if (map[y][x] == 0)
            continue;
          unsigned int plateauHeightNeighbourPoints = 0;
		  //define maximum acceptable number of nearby low-level coordinates to keep in map
          const unsigned int NEIGHBOURS_LIMIT = 6;
          yTop = y - 1;
          yBottom = y + 1;
          xLeft = x - 1;
          xRight = x + 1;
          for (auto nY = yTop; nY <= yBottom; nY++)
            {
              for (auto nX = xLeft; nX <= xRight; nX++)
                {
                  if (nY == y && nX == x) //don't need to check itself
                    continue;
                  if (map[nY][nX] <= plateauHeight)
                    ++plateauHeightNeighbourPoints;
                }
            }
          if (plateauHeightNeighbourPoints >= NEIGHBOURS_LIMIT)
            map[y][x] = 0;
        }
    }
}

/**
* @brief remove hill kernel points that were not lucky enough to get fattened
* @note technically these hills are invisible for player, but they affect camera movement when it hovers them
* and they unnecessary occupy GPU memory
*/
void HillsGenerator::removeOrphanHills()
{
  for (int y = 1; y < WORLD_HEIGHT; y++)
    {
      for (int x = 1; x < WORLD_WIDTH; x++)
        {
          if (map[y][x] != 0.0f &&
              map[y-1][x-1] == 0 &&
              map[y-1][x] == 0 &&
              map[y-1][x+1] == 0 &&
              map[y][x-1] == 0 &&
              map[y][x+1] == 0 &&
              map[y+1][x-1] == 0 &&
              map[y+1][x] == 0 &&
              map[y+1][x+1] == 0)
            map[y][x] = 0.0f;
        }
    }
}

/**
* @brief remove sink points in the hills regions.
*/
void HillsGenerator::smoothMapSinks()
{
  for (unsigned int y = 1; y < WORLD_HEIGHT; y++)
    {
      for (unsigned int x = 1; x < WORLD_WIDTH; x++)
        {
          unsigned int higherNeighbours = 0;
          const unsigned int NEIGHBOURS_LIMIT = 6;

		  //find how many neighbour points are higher...
          for (int yOffset = -1; yOffset <= 1; yOffset++)
            {
              for (int xOffset = -1; xOffset <= 1; xOffset++)
                {
                  if (yOffset == 0 && xOffset == 0) //don't need to check itself
                    continue;
                  higherNeighbours += (map[y][x] < map[y+yOffset][x+xOffset] ? 1 : 0);
                }
            }
		  //...if there are too much, level this point's height as average of its neighbours
          if (higherNeighbours >= NEIGHBOURS_LIMIT)
            {
              float avgHeight = map[y-1][x-1]
                               +map[y-1][x]
                               +map[y-1][x+1]
                               +map[y][x-1]
							   +map[y][x]
                               +map[y][x+1]
                               +map[y+1][x-1]
                               +map[y+1][x]
                               +map[y+1][x+1];
              avgHeight /= 9;
              map[y][x] = avgHeight;
            }
        }
    }
}

/**
* @brief create map of the tangent vectors for each coordinate according to existing normal map
*/
void HillsGenerator::createTangentMap()
{
  using glm::vec3;
  //reinitialize in case of recreation
  tangentMap.clear();
  tangentMap.reserve(WORLD_HEIGHT + 1);
  for (size_t row = 0; row < WORLD_HEIGHT + 1; row++)
    {
      vec3 defaultTangent(1.0f, 0.0f, 0.0f);
      std::vector<vec3> emptyVec(WORLD_WIDTH + 1, defaultTangent);
      tangentMap.emplace_back(emptyVec);
    }

  for (unsigned int y = 1; y < map.size() - 1; y++)
    {
      for (unsigned int x = 1; x < map[0].size() - 1; x++)
        tangentMap[y][x] = glm::normalize(glm::cross(normalMap[y][x], vec3(0.0f, 1.0f, 0.0f)));
    }
}

/**
* @brief create map of the bitangent vectors for each coordinate according to existing normal and tangent maps
*/
void HillsGenerator::createBitangentMap()
{
  using glm::vec3;
  //reinitialize in case of recreation
  bitangentMap.clear();
  bitangentMap.reserve(WORLD_HEIGHT + 1);
  for (size_t row = 0; row < WORLD_HEIGHT + 1; row++)
    {
      vec3 defaultBitangent(0.0f, 0.0f, 1.0f);
      std::vector<vec3> emptyVec(WORLD_WIDTH + 1, defaultBitangent);
      bitangentMap.emplace_back(emptyVec);
    }

  for (unsigned int y = 1; y < map.size() - 1; y++)
    {
      for (unsigned int x = 1; x < map[0].size() - 1; x++)
        bitangentMap[y][x] = glm::normalize(glm::cross(normalMap[y][x], tangentMap[y][x]));
    }
}

/**
* @brief plain ctor
* @note positioning is offset on both X and Y axis by the half of the world dimension
*/
HillsGenerator::HillVertex::HillVertex(glm::vec3 pos, glm::vec2 texCoords, glm::vec3 normal, glm::vec3 tangent, glm::vec3 bitangent) noexcept
  :
	position{pos.x - HALF_WORLD_WIDTH, pos.y, pos.z - HALF_WORLD_HEIGHT},
	texCoords{texCoords.x, texCoords.y},
	normal{normal.x, normal.y, normal.z},
	tangent{tangent.x, tangent.y, tangent.z},
	bitangent{bitangent.x, bitangent.y, bitangent.z}
{}
