#include "generator/WaterMapGenerator.h"

bool WaterMapGenerator::_WATER_ANIMATION_BENCHMARK_PASS_THROUGH = false;

WaterMapGenerator::WaterMapGenerator(Shader &waterShader)
  :
    MapGenerator(),
    waterShader(waterShader)
{}

WaterMapGenerator::~WaterMapGenerator()
{
  deleteGLObjects();
  glDeleteVertexArrays(1, &culledVAO);
  glDeleteBuffers(1, &culledVBO);
  glDeleteTransformFeedbacks(1, &TFBO);
  delete[] vertices;
}

void WaterMapGenerator::prepareMap()
{
  unsigned int numWaterTiles = 0;
  generateMap(SHORE_SIZE_BASE, WATER_LEVEL, numWaterTiles);
  while (numWaterTiles < TILES_WIDTH * (SHORE_SIZE_BASE + 2) * (SHORE_SIZE_BASE + 2) * 9
         || numWaterTiles > TILES_WIDTH * (SHORE_SIZE_BASE + 3) * (SHORE_SIZE_BASE + 3) * 9)
    {
      numWaterTiles = 0;
      initializeMap(map);
      generateMap(SHORE_SIZE_BASE, WATER_LEVEL, numWaterTiles);
    }
}

void WaterMapGenerator::postPrepareMap()
{
  initializeMap(postProcessMap);
  addWaterNearbyBaseTerrain();
  fillSharpTerrainWithWater();
  createTiles(true, false, postProcessMap, 0);
  tiles.shrink_to_fit();
}

void WaterMapGenerator::fillBufferData()
{
  numVertices = tiles.size() * 36;
  vertices = new GLfloat[numVertices];
  for (unsigned int i = 0; i < tiles.size(); i++)
    {
      TerrainTile& tile = tiles[i];
      int offset = i * 36;
      //ll1
      vertices[offset] = -1- HALF_TILES_WIDTH + tile.mapX;
      vertices[offset+1] = tile.lowLeft;
      vertices[offset+2] = - HALF_TILES_HEIGHT + tile.mapY;
      vertices[offset+3] = 0.0f;
      vertices[offset+4] = 1.0f;
      vertices[offset+5] = 0.0f;
      //lr1
      vertices[offset+6] = - HALF_TILES_WIDTH + tile.mapX;
      vertices[offset+7] = tile.lowRight;
      vertices[offset+8] = - HALF_TILES_HEIGHT + tile.mapY;
      vertices[offset+9] = 0.0f;
      vertices[offset+10] = 1.0f;
      vertices[offset+11] = 0.0f;
      //ur1
      vertices[offset+12] = - HALF_TILES_WIDTH + tile.mapX;
      vertices[offset+13] = tile.upperRight;
      vertices[offset+14] = -1 - HALF_TILES_HEIGHT + tile.mapY;
      vertices[offset+15] = 0.0f;
      vertices[offset+16] = 1.0f;
      vertices[offset+17] = 0.0f;
      //ur2
      vertices[offset+18] = - HALF_TILES_WIDTH + tile.mapX;
      vertices[offset+19] = tile.upperRight;
      vertices[offset+20] = -1 - HALF_TILES_HEIGHT + tile.mapY;
      vertices[offset+21] = 0.0f;
      vertices[offset+22] = 1.0f;
      vertices[offset+23] = 0.0f;
      //ul2
      vertices[offset+24] = -1 - HALF_TILES_WIDTH + tile.mapX;
      vertices[offset+25] = tile.upperLeft;
      vertices[offset+26] = -1 - HALF_TILES_HEIGHT + tile.mapY;
      vertices[offset+27] = 0.0f;
      vertices[offset+28] = 1.0f;
      vertices[offset+29] = 0.0f;
      //ll2
      vertices[offset+30] = -1- HALF_TILES_WIDTH + tile.mapX;
      vertices[offset+31] = tile.lowLeft;
      vertices[offset+32] = - HALF_TILES_HEIGHT + tile.mapY;
      vertices[offset+33] = 0.0f;
      vertices[offset+34] = 1.0f;
      vertices[offset+35] = 0.0f;
    }
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(GLfloat), vertices, GL_STREAM_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

  glCreateVertexArrays(1, &culledVAO);
  glBindVertexArray(culledVAO);
  glCreateBuffers(1, &culledVBO);
  glBindBuffer(GL_ARRAY_BUFFER, culledVBO);
  glNamedBufferStorage(culledVBO, numVertices * sizeof(GLfloat), 0, GL_NONE);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glCreateTransformFeedbacks(1, &TFBO);
  glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, TFBO);
  const GLchar* varyings[2] = {"o_pos", "o_normal"};
  glTransformFeedbackVaryings(waterShader.getID(), 2, varyings, GL_INTERLEAVED_ATTRIBS);
  waterShader.linkAgain();
  glTransformFeedbackBufferBase(TFBO, 0, culledVBO);
  resetAllGLBuffers();
}

void WaterMapGenerator::bufferVertices()
{
  glNamedBufferData(vbo, numVertices * sizeof(GLfloat), vertices, GL_STREAM_DRAW);
}

void WaterMapGenerator::addWaterNearbyBaseTerrain()
{
  //add water above the tile
  for (unsigned int y = 0; y < TILES_HEIGHT - 1; y++)
    {
      for (unsigned int x = 0; x < TILES_WIDTH; x++)
        {
          if (map[y+1][x] != 0)
            postProcessMap[y][x] = map[y+1][x];
        }
    }
  //add water below the tile
  for (unsigned int y = TILES_HEIGHT; y > 0; y--)
    {
      for (unsigned int x = 0; x < TILES_WIDTH; x++)
        {
          if (map[y-1][x] != 0)
            postProcessMap[y][x] = map[y-1][x];
        }
    }
  //add more water below the tile
  for (unsigned int y = TILES_HEIGHT; y > 0; y--)
    {
      for (unsigned int x = 0; x < TILES_WIDTH; x++)
        {
          if (postProcessMap[y-1][x] != 0)
            postProcessMap[y][x] = postProcessMap[y-1][x];
        }
    }
  //add water left to the tile
  for (unsigned int x = 0; x < TILES_WIDTH - 1; x++)
    {
      for (unsigned int y = 0; y < TILES_HEIGHT; y++)
        {
          if (map[y][x+1] != 0)
            postProcessMap[y][x] = map[y][x+1];
        }
    }
  //add water right to the tile
  for (unsigned int x = TILES_WIDTH; x > 0; x--)
    {
      for (unsigned int y = 0; y < TILES_HEIGHT; y++)
        {
          if (map[y][x-1] != 0)
            postProcessMap[y][x] = map[y][x-1];
        }
    }
  //add more water right to the tile
  for (unsigned int x = TILES_WIDTH; x > 0; x--)
    {
      for (unsigned int y = 0; y < TILES_HEIGHT; y++)
        {
          if (postProcessMap[y][x-1] != 0)
            postProcessMap[y][x] = postProcessMap[y][x-1];
        }
    }
}

void WaterMapGenerator::fillSharpTerrainWithWater()
{
  for (int y2 = 1; y2 < TILES_HEIGHT - 1; y2++)
    {
      for (int x2 = 1; x2 < TILES_WIDTH - 1; x2++)
        {
          if (postProcessMap[y2][x2] == WATER_LEVEL)
            continue;
          if ((postProcessMap[y2-1][x2] == WATER_LEVEL && postProcessMap[y2+1][x2] == WATER_LEVEL) || (postProcessMap[y2][x2-1] == WATER_LEVEL && postProcessMap[y2][x2+1] == WATER_LEVEL))
            postProcessMap[y2][x2] = WATER_LEVEL;
        }
    }
}

void WaterMapGenerator::liftWaterLevel(float liftValue)
{
  for (std::vector<float>& row : map)
    {
      for (float& height : row)
        {
          if (height < 0)
            height += liftValue;
        }
    }
}

void WaterMapGenerator::generateMap(int shoreSizeBase, float waterLevel, unsigned int &numWaterTiles)
{
  bool startAxisFromX = rand() % 2 == 0;
  bool riverEnd = false;
  unsigned int curveMaxDistance = rand() % 48 + 48;
  unsigned int curveDistanceStep = 0;
  int numCurveChanges = 0;
  enum DIRECTION : int {
    UP = 0, UP_RIGHT, RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT, UP_LEFT
  };
  DIRECTION dir;
  unsigned int y, x;
  unsigned int startCoord = rand() % TILES_HEIGHT;
  x = startAxisFromX ? startCoord : 0;
  y = startAxisFromX ? 0 : startCoord;
  dir = startAxisFromX ? DOWN : RIGHT;

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
            if (x >= TILES_WIDTH)
              {
                x = TILES_WIDTH;
                riverEnd = true;
              }

            map[y][x] = waterLevel;
            ++numWaterTiles;
            if (curveDistanceStep == curveMaxDistance)
              {
                numCurveChanges++;
                curveDistanceStep = 0;
                curveMaxDistance = rand() % 48 + 48;
                dir = (DIRECTION)(rand() % 8);
                while (dir != UP_LEFT && dir != UP_RIGHT)
                  dir = (DIRECTION)(rand() % 8);
              }
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
            if (x >= TILES_WIDTH)
              {
                x = TILES_WIDTH;
                riverEnd = true;
              }
            map[y][x] = waterLevel;
            ++numWaterTiles;
            if (curveDistanceStep == curveMaxDistance)
              {
                numCurveChanges++;
                curveDistanceStep = 0;
                curveMaxDistance = rand() % 48 + 48;
                dir = (DIRECTION)(rand() % 8);
                while (dir != UP && dir != RIGHT)
                  dir = (DIRECTION)(rand() % 8);
              }
            break;
          }
        case RIGHT:
          {
            ++curveDistanceStep;
            ++x;
            if (x >= TILES_WIDTH)
              {
                x = TILES_WIDTH;
                riverEnd = true;
              }
            if (rand() % 4 == 0)
              y += rand() % 2 == 0 ? 2 : -2;
            if (y <= 0)
              {
                y = 0;
                riverEnd = true;
              }
            if (y >= TILES_HEIGHT)
              {
                y = TILES_HEIGHT;
                riverEnd = true;
              }

            map[y][x] = waterLevel;
            ++numWaterTiles;
            if (curveDistanceStep == curveMaxDistance)
              {
                numCurveChanges++;
                curveDistanceStep = 0;
                curveMaxDistance = rand() % 48 + 48;
                dir = (DIRECTION)(rand() % 8);
                while (dir != UP_RIGHT && dir != DOWN_RIGHT)
                  dir = (DIRECTION)(rand() % 8);
              }
            break;
          }
        case DOWN_RIGHT:
          {
            ++curveDistanceStep;
            y += rand() % 2;
            x += rand() % 2;
            if (y >= TILES_HEIGHT)
              {
                y = TILES_HEIGHT;
                riverEnd = true;
              }
            if (x >= TILES_WIDTH)
              {
                x = TILES_WIDTH;
                riverEnd = true;
              }
            map[y][x] = waterLevel;
            ++numWaterTiles;
            if (curveDistanceStep == curveMaxDistance)
              {
                numCurveChanges++;
                curveDistanceStep = 0;
                curveMaxDistance = rand() % 48 + 48;
                dir = (DIRECTION)(rand() % 8);
                while (dir != DOWN && dir != RIGHT)
                  dir = (DIRECTION)(rand() % 8);
              }
            break;
          }
        case DOWN:
          {
            ++curveDistanceStep;
            ++y;
            if (y >= TILES_HEIGHT)
              {
                y = TILES_HEIGHT;
                riverEnd = true;
              }
            if (rand() % 4 == 0)
              x += rand() % 2 == 0 ? 2 : -2;
            if (x <= 0)
              {
                x = 0;
                riverEnd = true;
              }
            if (x >= TILES_WIDTH)
              {
                x = TILES_WIDTH;
                riverEnd = true;
              }

            map[y][x] = waterLevel;
            ++numWaterTiles;
            if (curveDistanceStep == curveMaxDistance)
              {
                numCurveChanges++;
                curveDistanceStep = 0;
                curveMaxDistance = rand() % 48 + 48;
                dir = (DIRECTION)(rand() % 8);
                while (dir != DOWN_RIGHT && dir != DOWN_LEFT)
                  dir = (DIRECTION)(rand() % 8);
              }
            break;
          }
        case DOWN_LEFT:
          {
            ++curveDistanceStep;
            y += rand() % 2;
            x -= rand() % 2;
            if (y >= TILES_HEIGHT)
              {
                y = TILES_HEIGHT;
                riverEnd = true;
              }
            if (x <= 0)
              {
                x = 0;
                riverEnd = true;
              }
            map[y][x] = waterLevel;
            ++numWaterTiles;
            if (curveDistanceStep == curveMaxDistance)
              {
                numCurveChanges++;
                curveDistanceStep = 0;
                curveMaxDistance = rand() % 48 + 48;
                dir = (DIRECTION)(rand() % 8);
                while (dir != DOWN && dir != LEFT)
                  dir = (DIRECTION)(rand() % 8);
              }
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
            if (y >= TILES_HEIGHT)
              {
                y = TILES_HEIGHT;
                riverEnd = true;
              }

            map[y][x] = waterLevel;
            ++numWaterTiles;
            if (curveDistanceStep == curveMaxDistance)
              {
                numCurveChanges++;
                curveDistanceStep = 0;
                curveMaxDistance = rand() % 48 + 48;
                dir = (DIRECTION)(rand() % 8);
                while (dir != UP_LEFT && dir != DOWN_LEFT)
                  dir = (DIRECTION)(rand() % 8);
              }
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
            map[y][x] = waterLevel;
            ++numWaterTiles;
            if (curveDistanceStep == curveMaxDistance)
              {
                numCurveChanges++;
                curveDistanceStep = 0;
                curveMaxDistance = rand() % 48 + 48;
                dir = (DIRECTION)(rand() % 8);
                while (dir != UP && dir != LEFT)
                  dir = (DIRECTION)(rand() % 8);
              }
            break;
          }
        default:
          break;
        }

      //fattening the kernel
      int shoreSizeYT = rand() % 2 + shoreSizeBase;
      int shoreSizeYB = rand() % 2 + shoreSizeBase;
      int shoreSizeXL = rand() % 2 + shoreSizeBase;
      int shoreSizeXR = rand() % 2 + shoreSizeBase;
      ++riverTileCounter;
      if (riverTileCounter % 19 == 0)
        {
          riverTileCounter = 0;
          shoreSizeOffset += shoreSizeIncrease ? 1 : -1;
          if (shoreSizeOffset > shoreSizeBase)
            shoreSizeIncrease = !shoreSizeIncrease;
          else if (shoreSizeOffset < 0)
            {
              shoreSizeOffset = 0;
              shoreSizeIncrease = !shoreSizeIncrease;
            }
        }
      int xl = x - shoreSizeXL - shoreSizeOffset;
      if (xl <= 0)
        xl = 0;
      int xr = x + shoreSizeXR + shoreSizeOffset;
      if (xr >= TILES_WIDTH)
        xr = TILES_WIDTH;
      int yt = y - shoreSizeYT - shoreSizeOffset;
      if (yt <= 0)
        yt = 0;
      int yb = y + shoreSizeYB + shoreSizeOffset;
      if (yb >= TILES_HEIGHT)
        yb = TILES_HEIGHT;
      for (int y1 = yt; y1 <= yb; y1++)
        {
          for (int x1 = xl; x1 <= xr; x1++)
            {
              map[y1][x1] = waterLevel;
              ++numWaterTiles;
            }
        }
    }
}

GLfloat *WaterMapGenerator::getHeightOffsets()
{
  return waterHeightOffsets;
}

void WaterMapGenerator::updateAnimationFrame(Options& options)
{
  BENCHMARK_PASS_CHECK("(SI)Water: Update animation frame", true, _WATER_ANIMATION_BENCHMARK_PASS_THROUGH);
  _WATER_ANIMATION_BENCHMARK_PASS_THROUGH = true;
  double frameTime = glfwGetTime();
  double offsetMultiplier = frameTime * 0.1;
  for (size_t i = 0; i < WATER_HEIGHT_OFFSETS_SIZE; i+=2)
    {
        waterHeightOffsets[i] = std::cos(offsetMultiplier * (i % 31)) * 0.0625 + WATER_LEVEL;
        waterHeightOffsets[i+1] = std::sin(offsetMultiplier * (i % 29)) * 0.0689 + WATER_LEVEL;
    }
  unsigned int numWaterTiles = tiles.size();
  glm::vec3 normalLR, normalUL;
  for (unsigned int i = 0; i < numWaterTiles; ++i)
    {
      if (options.get(RECREATE_TERRAIN_REQUEST))
        return;
      TerrainTile& tile = tiles[i];
      unsigned int pointerOffsetWithStride = i * 36;
      unsigned int heightOffsetWithStrideForLow = (tile.mapY+1) * TILES_WIDTH;
      unsigned int heightOffsetWithStrideForUpper = tile.mapY * TILES_WIDTH;

      float ll = waterHeightOffsets[heightOffsetWithStrideForLow + tile.mapX];
      float lr = waterHeightOffsets[heightOffsetWithStrideForLow + tile.mapX + 1];
      float ur = waterHeightOffsets[heightOffsetWithStrideForUpper + tile.mapX + 1];
      float ul = waterHeightOffsets[heightOffsetWithStrideForUpper + tile.mapX];

      normalLR = glm::vec3(ll - lr, 1, ur - lr); //just do the calculations manually (UR-LR x LL-LR and LL-UL x UR-UL)
      normalUL = glm::vec3(ul - ur, 1, ul - ll);

      *(vertices+1+pointerOffsetWithStride) = ll;
      *(vertices+7+pointerOffsetWithStride) = lr;
      *(vertices+13+pointerOffsetWithStride) = ur;
      *(vertices+19+pointerOffsetWithStride) = ur;
      *(vertices+25+pointerOffsetWithStride) = ul;
      *(vertices+31+pointerOffsetWithStride) = ll;

      *(vertices+3+pointerOffsetWithStride) = normalLR.x;
      *(vertices+5+pointerOffsetWithStride) = normalLR.z;
      *(vertices+9+pointerOffsetWithStride) = normalLR.x;
      *(vertices+11+pointerOffsetWithStride) = normalLR.z;
      *(vertices+15+pointerOffsetWithStride) = normalLR.x;
      *(vertices+17+pointerOffsetWithStride) = normalLR.z;

      *(vertices+21+pointerOffsetWithStride) = normalUL.x;
      *(vertices+23+pointerOffsetWithStride) = normalUL.z;
      *(vertices+27+pointerOffsetWithStride) = normalUL.x;
      *(vertices+29+pointerOffsetWithStride) = normalUL.z;
      *(vertices+33+pointerOffsetWithStride) = normalUL.x;
      *(vertices+35+pointerOffsetWithStride) = normalUL.z;
    }
}

GLuint WaterMapGenerator::getCulledVAO() const
{
  return culledVAO;
}

GLuint WaterMapGenerator::getTransformFeedback() const
{
  return TFBO;
}

void WaterMapGenerator::_setWaterAnimationBenchmarkPassThrough(bool passThru)
{
  _WATER_ANIMATION_BENCHMARK_PASS_THROUGH = passThru;
}
