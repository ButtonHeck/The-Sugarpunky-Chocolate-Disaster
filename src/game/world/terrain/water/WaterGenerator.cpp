#include "game/world/terrain/water/WaterGenerator.h"

WaterGenerator::WaterGenerator(WaterShader &shaders)
  :
    Generator(),
    culledBuffers(VAO | VBO | TFBO),
    shaders(shaders),
    heightOffsets(new GLfloat[WATER_HEIGHT_OFFSETS_SIZE])
{}

void WaterGenerator::setup()
{
  generateMap();
  while (numTiles < WORLD_WIDTH * (SHORE_SIZE_BASE + 2) * (SHORE_SIZE_BASE + 2) * 9
         || numTiles > WORLD_WIDTH * (SHORE_SIZE_BASE + 3) * (SHORE_SIZE_BASE + 3) * 9)
    {
      initializeMap(map);
      generateMap();
    }
}

void WaterGenerator::setupConsiderTerrain()
{
  initializeMap(postProcessMap);
  for (unsigned int i = 0; i < 4; i++)
    addWaterNearbyTerrain();
  createTiles(true, false, postProcessMap, 0);
  tiles.shrink_to_fit();
  fillBufferData();
}

void WaterGenerator::fillBufferData()
{
  numVertices = tiles.size() * 24;
  vertices.reset(new GLfloat[numVertices]);
  const size_t INDICES_DATA_LENGTH = tiles.size() * VERTICES_PER_TILE;
  std::unique_ptr<GLuint[]> indices(new GLuint[INDICES_DATA_LENGTH]);
  size_t indicesBufferIndex = 0;
  const static glm::vec3 DEFAULT_Y_NORMAL(0.0f, 1.0f, 0.0f);

  for (unsigned int i = 0; i < tiles.size(); i++)
    {
      TerrainTile& tile = tiles[i];
      int x = tile.mapX, y = tile.mapY;

      WaterVertex lowLeft(glm::vec3(x - 1, tile.lowLeft, y), DEFAULT_Y_NORMAL);
      WaterVertex lowRight(glm::vec3(x, tile.lowRight, y), DEFAULT_Y_NORMAL);
      WaterVertex upRight(glm::vec3(x, tile.upperRight, y - 1), DEFAULT_Y_NORMAL);
      WaterVertex upLeft(glm::vec3(x - 1, tile.upperLeft, y - 1), DEFAULT_Y_NORMAL);

      int vertexBufferOffset = i * 24;
      bufferVertex(vertices.get(), vertexBufferOffset+0, lowLeft); //ll1
      bufferVertex(vertices.get(), vertexBufferOffset+6, lowRight); //lr1
      bufferVertex(vertices.get(), vertexBufferOffset+12, upRight); //ur1
      bufferVertex(vertices.get(), vertexBufferOffset+18, upLeft); //ur2

      GLuint indicesBufferBaseVertex = i * 4;
      indices[indicesBufferIndex++] = indicesBufferBaseVertex + 0;
      indices[indicesBufferIndex++] = indicesBufferBaseVertex + 1;
      indices[indicesBufferIndex++] = indicesBufferBaseVertex + 2;
      indices[indicesBufferIndex++] = indicesBufferBaseVertex + 2;
      indices[indicesBufferIndex++] = indicesBufferBaseVertex + 3;
      indices[indicesBufferIndex++] = indicesBufferBaseVertex + 0;
    }
  basicGLBuffers.bind(VAO | VBO | EBO);
  glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(GLfloat), vertices.get(), GL_STREAM_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * INDICES_DATA_LENGTH, indices.get(), GL_STATIC_DRAW);
  setupGLBufferAttributes();

  if (culledBuffers.get(VAO) != 0)
    culledBuffers.deleteBuffers();
  culledBuffers.create(VAO | VBO | TFBO);
  culledBuffers.bind(VAO | VBO | TFBO);
  glNamedBufferStorage(culledBuffers.get(VBO), numVertices * sizeof(GLfloat), 0, GL_NONE);
  setupGLBufferAttributes();
  shaders.setupCulling();
  glTransformFeedbackBufferBase(culledBuffers.get(TFBO), 0, culledBuffers.get(VBO));
  resetAllGLBuffers();
}

void WaterGenerator::bufferNewData()
{
  glNamedBufferData(basicGLBuffers.get(VBO), numVertices * sizeof(GLfloat), vertices.get(), GL_STREAM_DRAW);
}

void WaterGenerator::addWaterNearbyTerrain()
{
  //add water above the tile
  for (unsigned int y = 0; y < WORLD_HEIGHT - 1; y++)
    {
      for (unsigned int x = 0; x < WORLD_WIDTH; x++)
        {
          if (map[y+1][x] != 0)
            postProcessMap[y][x] = map[y+1][x];
        }
    }
  //add more water above the tile
  for (unsigned int y = 0; y < WORLD_HEIGHT - 1; y++)
    {
      for (unsigned int x = 0; x < WORLD_WIDTH; x++)
        {
          if (postProcessMap[y+1][x] != 0)
            postProcessMap[y][x] = postProcessMap[y+1][x];
        }
    }

  //add water below the tile
  for (unsigned int y = WORLD_HEIGHT; y > 0; y--)
    {
      for (unsigned int x = 0; x < WORLD_WIDTH; x++)
        {
          if (map[y-1][x] != 0)
            postProcessMap[y][x] = map[y-1][x];
        }
    }
  //add more water below the tile
  for (unsigned int y = WORLD_HEIGHT; y > 0; y--)
    {
      for (unsigned int x = 0; x < WORLD_WIDTH; x++)
        {
          if (postProcessMap[y-1][x] != 0)
            postProcessMap[y][x] = postProcessMap[y-1][x];
        }
    }

  //add water left to the tile
  for (unsigned int x = 0; x < WORLD_WIDTH - 1; x++)
    {
      for (unsigned int y = 0; y < WORLD_HEIGHT; y++)
        {
          if (map[y][x+1] != 0)
            postProcessMap[y][x] = map[y][x+1];
        }
    }
  //add more water left to the tile
  for (unsigned int x = 0; x < WORLD_WIDTH - 1; x++)
    {
      for (unsigned int y = 0; y < WORLD_HEIGHT; y++)
        {
          if (postProcessMap[y][x+1] != 0)
            postProcessMap[y][x] = postProcessMap[y][x+1];
        }
    }

  //add water right to the tile
  for (unsigned int x = WORLD_WIDTH; x > 0; x--)
    {
      for (unsigned int y = 0; y < WORLD_HEIGHT; y++)
        {
          if (map[y][x-1] != 0)
            postProcessMap[y][x] = map[y][x-1];
        }
    }
  //add more water right to the tile
  for (unsigned int x = WORLD_WIDTH; x > 0; x--)
    {
      for (unsigned int y = 0; y < WORLD_HEIGHT; y++)
        {
          if (postProcessMap[y][x-1] != 0)
            postProcessMap[y][x] = postProcessMap[y][x-1];
        }
    }
}

void WaterGenerator::generateMap()
{
  numTiles = 0;
  bool startAxisFromX = rand() % 2 == 0;
  bool riverEnd = false;
  unsigned int curveMaxDistance = rand() % 48 + 48;
  unsigned int curveDistanceStep = 0;
  int numCurveChanges = 0;
  int y, x;
  unsigned int startCoord = rand() % WORLD_HEIGHT;
  x = startAxisFromX ? startCoord : 0;
  y = startAxisFromX ? 0 : startCoord;
  DIRECTION dir = startAxisFromX ? DOWN : RIGHT;
  int shoreSizeOffset = 0, riverTileCounter = 0;
  bool shoreSizeIncrease = true;

  while (!riverEnd)
    {
      switch (dir) {
        case UP:
          {
            ++curveDistanceStep;
            --y;
            if (y <= 0)
              {
                y = 0;
                riverEnd = true;
              }
            if (rand() % 4 == 0)
              x += rand() % 2 == 0 ? 2 : -2;
            if (x <= 0)
              {
                x = 0;
                riverEnd = true;
              }
            if (x >= WORLD_WIDTH)
              {
                x = WORLD_WIDTH;
                riverEnd = true;
              }
            setNewDirection(x, y, numCurveChanges, curveDistanceStep, curveMaxDistance, dir, UP_LEFT, UP_RIGHT);
            break;
          }
        case UP_RIGHT:
          {
            ++curveDistanceStep;
            y -= rand() % 2;
            x += rand() % 2;
            if (y <= 0)
              {
                y = 0;
                riverEnd = true;
              }
            if (x >= WORLD_WIDTH)
              {
                x = WORLD_WIDTH;
                riverEnd = true;
              }
            setNewDirection(x, y, numCurveChanges, curveDistanceStep, curveMaxDistance, dir, UP, RIGHT);
            break;
          }
        case RIGHT:
          {
            ++curveDistanceStep;
            ++x;
            if (x >= WORLD_WIDTH)
              {
                x = WORLD_WIDTH;
                riverEnd = true;
              }
            if (rand() % 4 == 0)
              y += rand() % 2 == 0 ? 2 : -2;
            if (y <= 0)
              {
                y = 0;
                riverEnd = true;
              }
            if (y >= WORLD_HEIGHT)
              {
                y = WORLD_HEIGHT;
                riverEnd = true;
              }
            setNewDirection(x, y, numCurveChanges, curveDistanceStep, curveMaxDistance, dir, UP_RIGHT, DOWN_RIGHT);
            break;
          }
        case DOWN_RIGHT:
          {
            ++curveDistanceStep;
            y += rand() % 2;
            x += rand() % 2;
            if (y >= WORLD_HEIGHT)
              {
                y = WORLD_HEIGHT;
                riverEnd = true;
              }
            if (x >= WORLD_WIDTH)
              {
                x = WORLD_WIDTH;
                riverEnd = true;
              }
            setNewDirection(x, y, numCurveChanges, curveDistanceStep, curveMaxDistance, dir, DOWN, RIGHT);
            break;
          }
        case DOWN:
          {
            ++curveDistanceStep;
            ++y;
            if (y >= WORLD_HEIGHT)
              {
                y = WORLD_HEIGHT;
                riverEnd = true;
              }
            if (rand() % 4 == 0)
              x += rand() % 2 == 0 ? 2 : -2;
            if (x <= 0)
              {
                x = 0;
                riverEnd = true;
              }
            if (x >= WORLD_WIDTH)
              {
                x = WORLD_WIDTH;
                riverEnd = true;
              }
            setNewDirection(x, y, numCurveChanges, curveDistanceStep, curveMaxDistance, dir, DOWN_RIGHT, DOWN_LEFT);
            break;
          }
        case DOWN_LEFT:
          {
            ++curveDistanceStep;
            y += rand() % 2;
            x -= rand() % 2;
            if (y >= WORLD_HEIGHT)
              {
                y = WORLD_HEIGHT;
                riverEnd = true;
              }
            if (x <= 0)
              {
                x = 0;
                riverEnd = true;
              }
            setNewDirection(x, y, numCurveChanges, curveDistanceStep, curveMaxDistance, dir, DOWN, LEFT);
            break;
          }
        case LEFT:
          {
            ++curveDistanceStep;
            --x;
            if (x <= 0)
              {
                x = 0;
                riverEnd = true;
              }
            if (rand() % 4 == 0)
              y += rand() % 2 == 0 ? 2 : -2;
            if (y <= 0)
              {
                y = 0;
                riverEnd = true;
              }
            if (y >= WORLD_HEIGHT)
              {
                y = WORLD_HEIGHT;
                riverEnd = true;
              }
            setNewDirection(x, y, numCurveChanges, curveDistanceStep, curveMaxDistance, dir, UP_LEFT, DOWN_LEFT);
            break;
          }
        case UP_LEFT:
          {
            ++curveDistanceStep;
            y -= rand() % 2;
            x -= rand() % 2;
            if (x <= 0)
              {
                x = 0;
                riverEnd = true;
              }
            if (y <= 0)
              {
                y = 0;
                riverEnd = true;
              }
            setNewDirection(x, y, numCurveChanges, curveDistanceStep, curveMaxDistance, dir, UP, LEFT);
            break;
          }
        default:
          break;
      }
      fattenKernel(x, y, riverTileCounter, shoreSizeOffset, shoreSizeIncrease);
    }
}

void WaterGenerator::setNewDirection(int x, int y,
                                 int &numCurveChanges,
                                 unsigned int &curveDistanceStep,
                                 unsigned int &curveMaxDistance,
                                 DIRECTION &dir, DIRECTION limit1, DIRECTION limit2)
{
  map[y][x] = WATER_LEVEL;
  ++numTiles;
  if (curveDistanceStep == curveMaxDistance)
    {
      ++numCurveChanges;
      curveDistanceStep = 0;
      curveMaxDistance = rand() % 48 + 48;
      dir = (DIRECTION)(rand() % 8);
      while(dir != limit1 && dir != limit2)
        dir = (DIRECTION)(rand() % 8);
    }
}

void WaterGenerator::fattenKernel(int x, int y,
                                     int& riverTileCounter, int& shoreSizeOffset, bool& shoreSizeIncrease)
{
  int shoreSizeYT = rand() % 2 + SHORE_SIZE_BASE;
  int shoreSizeYB = rand() % 2 + SHORE_SIZE_BASE;
  int shoreSizeXL = rand() % 2 + SHORE_SIZE_BASE;
  int shoreSizeXR = rand() % 2 + SHORE_SIZE_BASE;
  ++riverTileCounter;
  if (riverTileCounter % 19 == 0)
    {
      riverTileCounter = 0;
      shoreSizeOffset += shoreSizeIncrease ? 1 : -1;
      if (shoreSizeOffset > SHORE_SIZE_BASE)
        shoreSizeIncrease = !shoreSizeIncrease;
      else if (shoreSizeOffset < 0)
        {
          shoreSizeOffset = 0;
          shoreSizeIncrease = !shoreSizeIncrease;
        }
    }
  int xLeft = ((int)(x-shoreSizeXL-shoreSizeOffset) <= 0 ? 0 : x-shoreSizeXL-shoreSizeOffset);
  int xRight = ((int)(x+shoreSizeXR+shoreSizeOffset) >= WORLD_WIDTH ? WORLD_WIDTH : x+shoreSizeXR+shoreSizeOffset);
  int yTop = ((int)(y-shoreSizeYT-shoreSizeOffset) <= 0 ? 0 : y-shoreSizeYT-shoreSizeOffset);
  int yBottom = ((int)(y+shoreSizeYB+shoreSizeOffset) >= WORLD_HEIGHT ? WORLD_HEIGHT : y+shoreSizeYB+shoreSizeOffset);
  for (int y1 = yTop; y1 <= yBottom; y1++)
    {
      for (int x1 = xLeft; x1 <= xRight; x1++)
        {
          map[y1][x1] = WATER_LEVEL;
          ++numTiles;
        }
    }
}

void WaterGenerator::updateAnimationFrame(double time, Options& options)
{
  BENCHMARK("(ST)Water: Update animation frame", true);
  using glm::vec3;
  using glm::vec4;
  double offsetMultiplier = time * 0.12;
  for (size_t i = 0; i < WATER_HEIGHT_OFFSETS_SIZE; i+=2)
    {
      heightOffsets[i] = std::cos(offsetMultiplier * ((i * i) % 19)) * 0.0825f + WATER_LEVEL;
      heightOffsets[i+1] = std::sin(offsetMultiplier * ((i * i) % 29)) * 0.0889f + WATER_LEVEL;
    }

  static bool mapCreated = false;
  if (!mapCreated)
    {
      normalMap.clear();
      normalMap.reserve(WORLD_HEIGHT + 1);
      for (size_t row = 0; row < WORLD_HEIGHT + 1; row++)
        {
          vec3 defaultNormal(0.0f, 1.0f, 0.0f);
          normalMap.emplace_back(WORLD_WIDTH + 1, defaultNormal);
        }
      mapCreated = true;
    }

  constexpr float NORMAL_Y_APPROX = 0.5f; //fake "true" normal calculation (use no sqrt and pow)
  for (unsigned int y = 1; y < WORLD_HEIGHT - 1; y++)
    {
      for (unsigned int x = 1; x < WORLD_WIDTH - 1; x++)
        {
          vec3 n0 = glm::normalize(vec3(heightOffsets[y * WORLD_WIDTH + x-1] - heightOffsets[y * WORLD_WIDTH + x],
                                        NORMAL_Y_APPROX,
                                        heightOffsets[(y-1) * WORLD_WIDTH + x] - heightOffsets[y * WORLD_WIDTH + x]));
          vec3 n3 = glm::normalize(vec3(heightOffsets[y * WORLD_WIDTH + x] - heightOffsets[y * WORLD_WIDTH + x+1],
                                        NORMAL_Y_APPROX,
                                        heightOffsets[(y-1) * WORLD_WIDTH + x+1] - heightOffsets[y * WORLD_WIDTH + x+1]));
          vec3 n6 = glm::normalize(vec3(heightOffsets[(y+1) * WORLD_WIDTH + x-1] - heightOffsets[(y+1) * WORLD_WIDTH + x],
                                        NORMAL_Y_APPROX,
                                        heightOffsets[y * WORLD_WIDTH + x] - heightOffsets[(y+1) * WORLD_WIDTH + x]));
          vec3 n1 = glm::normalize(vec3(heightOffsets[(y-1) * WORLD_WIDTH + x] - heightOffsets[(y-1) * WORLD_WIDTH + x+1],
                                        NORMAL_Y_APPROX,
                                        heightOffsets[(y-1) * WORLD_WIDTH + x] - heightOffsets[y * WORLD_WIDTH + x]));
          vec3 n4 = glm::normalize(vec3(heightOffsets[y * WORLD_WIDTH + x] - heightOffsets[y * WORLD_WIDTH + x+1],
                                        NORMAL_Y_APPROX,
                                        heightOffsets[y * WORLD_WIDTH + x] - heightOffsets[(y+1) * WORLD_WIDTH + x]));
          vec3 n9 = glm::normalize(vec3(heightOffsets[y * WORLD_WIDTH + x-1] - heightOffsets[y * WORLD_WIDTH + x],
                                        NORMAL_Y_APPROX,
                                        heightOffsets[y * WORLD_WIDTH + x-1] - heightOffsets[(y+1) * WORLD_WIDTH + x-1]));
          vec3 avgNormal = n0 + n1 + n3 + n4 + n6 + n9; //intentionally left unnormalized
          normalMap[y][x] = avgNormal;
        }
    }

  unsigned int numWaterTiles = tiles.size();
  for (unsigned int i = 0; i < numWaterTiles; ++i)
    {
      if (options[OPT_RECREATE_TERRAIN_REQUEST])
        return;
      TerrainTile& tile = tiles[i];
      unsigned int pointerOffsetWithStride = i * 24;
      unsigned int heightOffsetWithStrideForLow = (tile.mapY+1) * WORLD_WIDTH;
      unsigned int heightOffsetWithStrideForUpper = tile.mapY * WORLD_WIDTH;

      float ll = heightOffsets[heightOffsetWithStrideForLow + tile.mapX];
      float lr = heightOffsets[heightOffsetWithStrideForLow + tile.mapX + 1];
      float ur = heightOffsets[heightOffsetWithStrideForUpper + tile.mapX + 1];
      float ul = heightOffsets[heightOffsetWithStrideForUpper + tile.mapX];
      updateTileY(vertices.get(), pointerOffsetWithStride, vec4(ll, lr, ur, ul));

      updateVertexNormal(vertices.get(), pointerOffsetWithStride+3, normalMap[tile.mapY][tile.mapX]);
      updateVertexNormal(vertices.get(), pointerOffsetWithStride+9, normalMap[tile.mapY][tile.mapX+1]);
      updateVertexNormal(vertices.get(), pointerOffsetWithStride+15, normalMap[tile.mapY-1][tile.mapX+1]);
      updateVertexNormal(vertices.get(), pointerOffsetWithStride+21, normalMap[tile.mapY-1][tile.mapX]);
    }
}

void WaterGenerator::bufferVertex(GLfloat *vertices, int offset, WaterGenerator::WaterVertex vertex)
{
  vertices[offset+0] = vertex.posX;
  vertices[offset+1] = vertex.posY;
  vertices[offset+2] = vertex.posZ;
  vertices[offset+3] = vertex.normalX;
  vertices[offset+4] = vertex.normalY;
  vertices[offset+5] = vertex.normalZ;
}

void WaterGenerator::updateVertexNormal(GLfloat *vertices, int offset, glm::vec3& normal)
{
  vertices[offset+0] = normal.x;
  vertices[offset+1] = normal.y;
  vertices[offset+2] = normal.z;
}

void WaterGenerator::updateTileY(GLfloat *vertices, int offset, glm::vec4&& heights)
{
  vertices[offset+1] = heights.x;
  vertices[offset+7] = heights.y;
  vertices[offset+13] = heights.z;
  vertices[offset+19] = heights.w;
}

void WaterGenerator::setupGLBufferAttributes()
{
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
}

WaterGenerator::WaterVertex::WaterVertex(glm::vec3 position, glm::vec3 normal)
  :
    posX(position.x - HALF_WORLD_WIDTH), posY(position.y), posZ(position.z - HALF_WORLD_HEIGHT),
    normalX(normal.x), normalY(normal.y), normalZ(normal.z)
{}
