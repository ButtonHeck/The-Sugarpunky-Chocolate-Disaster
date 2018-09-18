#include "generator/WaterMapGenerator.h"

bool WaterMapGenerator::_WATER_ANIMATION_BENCHMARK_PASS_THROUGH = false;

WaterMapGenerator::WaterMapGenerator(Shader &waterShader)
  :
    MapGenerator(),
    waterShader(waterShader)
{
  waterHeightOffsets = new GLfloat[WATER_HEIGHT_OFFSETS_SIZE];
}

WaterMapGenerator::~WaterMapGenerator()
{
  deleteGLObjects();
  glDeleteVertexArrays(1, &culledVAO);
  glDeleteBuffers(1, &culledVBO);
  glDeleteTransformFeedbacks(1, &TFBO);
  delete[] vertices;
  delete[] waterHeightOffsets;
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
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(GLfloat), vertices, GL_STREAM_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

  if (culledVAO != 0)
    {
      glDeleteVertexArrays(1, &culledVAO);
      glDeleteBuffers(1, &culledVBO);
      glDeleteTransformFeedbacks(1, &TFBO);
    }
  glCreateVertexArrays(1, &culledVAO);
  glCreateBuffers(1, &culledVBO);
  glCreateTransformFeedbacks(1, &TFBO);
  glBindVertexArray(culledVAO);
  glBindBuffer(GL_ARRAY_BUFFER, culledVBO);
  glNamedBufferStorage(culledVBO, numVertices * sizeof(GLfloat), 0, GL_NONE);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
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
  BENCHMARK_PASS_CHECK("(SI/ST)Water: Update animation frame", true, _WATER_ANIMATION_BENCHMARK_PASS_THROUGH);
  _WATER_ANIMATION_BENCHMARK_PASS_THROUGH = true;
  double frameTime = glfwGetTime();
  double offsetMultiplier = frameTime * 0.12;
  for (size_t i = 0; i < WATER_HEIGHT_OFFSETS_SIZE; i+=2)
    {
        waterHeightOffsets[i] = std::cos(offsetMultiplier * ((i * i) % 19)) * 0.0825 + WATER_LEVEL;
        waterHeightOffsets[i+1] = std::sin(offsetMultiplier * ((i * i) % 29)) * 0.0889 + WATER_LEVEL;
    }
  unsigned int numWaterTiles = tiles.size();
  for (unsigned int i = 0; i < numWaterTiles; ++i)
    {
      if (options.get(RECREATE_TERRAIN_REQUEST))
        return;
      TerrainTile& tile = tiles[i];
      unsigned int pointerOffsetWithStride = i * 36;
      unsigned int heightOffsetWithStrideForLow = (tile.mapY+1) * TILES_WIDTH;
      unsigned int heightOffsetWithStrideForUpper = tile.mapY * TILES_WIDTH;

      if(tile.mapX > 0 && tile.mapX + 2 < TILES_WIDTH && tile.mapY - 2 > 0 && tile.mapY + 1 < TILES_HEIGHT)
        {
          float ll = waterHeightOffsets[heightOffsetWithStrideForLow + tile.mapX];
          float lr = waterHeightOffsets[heightOffsetWithStrideForLow + tile.mapX + 1];
          float ur = waterHeightOffsets[heightOffsetWithStrideForUpper + tile.mapX + 1];
          float ul = waterHeightOffsets[heightOffsetWithStrideForUpper + tile.mapX];
          float ll_xm1 = waterHeightOffsets[heightOffsetWithStrideForLow + tile.mapX - 1];
          float ll_yp1 = waterHeightOffsets[heightOffsetWithStrideForLow + tile.mapX + TILES_WIDTH];
          float ll_xm1_yp1 = waterHeightOffsets[heightOffsetWithStrideForLow + tile.mapX + TILES_WIDTH - 1];
          float lr_yp1 = waterHeightOffsets[heightOffsetWithStrideForLow + tile.mapX + TILES_WIDTH + 1];
          float lr_xp1 = waterHeightOffsets[heightOffsetWithStrideForLow + tile.mapX + 2];
          float ur_xp1 = waterHeightOffsets[heightOffsetWithStrideForUpper + tile.mapX + 2];
          float ur_ym1 = waterHeightOffsets[heightOffsetWithStrideForUpper + tile.mapX - TILES_WIDTH + 1];
          float ur_xp1_ym1 = waterHeightOffsets[heightOffsetWithStrideForUpper + tile.mapX - TILES_WIDTH + 2];
          float ul_ym1 = waterHeightOffsets[heightOffsetWithStrideForUpper + tile.mapX - TILES_WIDTH];
          float ul_xm1 = waterHeightOffsets[heightOffsetWithStrideForUpper + tile.mapX - 1];

          glm::vec3 normalLL3 =  glm::vec3(ll - lr, 1 - glm::sqrt(glm::pow(ll - lr, 2) + glm::pow(ur - lr, 2)), ur - lr);
          glm::vec3 normalLL12 = glm::vec3(ul - ur, 1 - glm::sqrt(glm::pow(ul - ur, 2) + glm::pow(ul - ll, 2)), ul - ll);
          glm::vec3 normalLL0 =  glm::vec3(ll_xm1 - ll, 1 - glm::sqrt(glm::pow(ll_xm1 - ll, 2) + glm::pow(ul - ll, 2)), ul - ll);
          glm::vec3 normalLL9 =  glm::vec3(ll_xm1 - ll, 1 - glm::sqrt(glm::pow(ll_xm1 - ll, 2) + glm::pow(ll_xm1 - ll_xm1_yp1, 2)), ll_xm1 - ll_xm1_yp1);
          glm::vec3 normalLL6 =  glm::vec3(ll_xm1_yp1 - ll_yp1, 1 - glm::sqrt(glm::pow(ll_xm1_yp1 - ll_yp1, 2) + glm::pow(ll - ll_yp1, 2)), ll - ll_yp1);
          glm::vec3 normalLL4 =  glm::vec3(ll - lr, 1 - glm::sqrt(glm::pow(ll - lr, 2) + glm::pow(ll - ll_yp1, 2)), ll - ll_yp1);
          glm::vec3 normalLL = normalLL3 + normalLL12 + normalLL0 + normalLL9 + normalLL6 + normalLL4;

          glm::vec3 normalLR3 =  glm::vec3(lr - lr_xp1, 1 - glm::sqrt(glm::pow(lr - lr_xp1, 2) + glm::pow(ur_xp1 - lr_xp1, 2)) , ur_xp1 - lr_xp1);
          glm::vec3 normalLR12 = glm::vec3(ur - ur_xp1, 1 - glm::sqrt(glm::pow(ur - ur_xp1, 2) + glm::pow(ur - lr, 2)), ur - lr);
          glm::vec3 normalLR0 = normalLL3;
          glm::vec3 normalLR9 = normalLL4;
          glm::vec3 normalLR6 =  glm::vec3(ll_yp1 - lr_yp1, 1 - glm::sqrt(glm::pow(ll_yp1 - lr_yp1, 2) + glm::pow(lr - lr_yp1, 2)), lr - lr_yp1);
          glm::vec3 normalLR4 =  glm::vec3(lr - lr_xp1, 1 - glm::sqrt(glm::pow(lr - lr_xp1, 2) + glm::pow(lr - lr_xp1, 2)), lr - lr_xp1);
          glm::vec3 normalLR = normalLR3 + normalLR12 + normalLR0 + normalLR9 + normalLR6 + normalLR4;

          glm::vec3 normalUR3 =  glm::vec3(ur - ur_xp1, 1 - glm::sqrt(glm::pow(ur - ur_xp1, 2) + glm::pow(ur_xp1_ym1 - ur_xp1, 2)), ur_xp1_ym1 - ur_xp1);
          glm::vec3 normalUR12 = glm::vec3(ur_ym1 - ur_xp1_ym1, 1 - glm::sqrt(glm::pow(ur_ym1 - ur_xp1_ym1, 2) + glm::pow(ur_ym1 - ur, 2)), ur_ym1 - ur);
          glm::vec3 normalUR0 =  glm::vec3(ul - ur, 1 - glm::sqrt(glm::pow(ul - ur, 2) + glm::pow(ur_ym1 - ur, 2)), ur_ym1 - ur);
          glm::vec3 normalUR9 = normalLL12;
          glm::vec3 normalUR6 = normalLL3;
          glm::vec3 normalUR4 = normalLR12;
          glm::vec3 normalUR = normalUR3 + normalUR12 + normalUR0 + normalUR9 + normalUR6 + normalUR4;

          glm::vec3 normalUL3 = normalUR0;
          glm::vec3 normalUL12 = glm::vec3(ul_ym1 - ur_ym1, 1 - glm::sqrt(glm::pow(ul_ym1 - ur_ym1, 2) + glm::pow(ul_ym1 - ul, 2)), ul_ym1 - ul);
          glm::vec3 normalUL0 =  glm::vec3(ul_xm1 - ul, 1 - glm::sqrt(glm::pow(ul_xm1 - ul, 2) + glm::pow(ul_ym1 - ul, 2)), ul_ym1 - ul);
          glm::vec3 normalUL9 =  glm::vec3(ul_xm1 - ul, 1 - glm::sqrt(glm::pow(ul_xm1 - ul, 2) + glm::pow(ul_xm1 - ll_xm1, 2)), ul_xm1 - ll_xm1);
          glm::vec3 normalUL6 = normalLL0;
          glm::vec3 normalUL4 = normalLL12;
          glm::vec3 normalUL = normalUL3 + normalUL12 + normalUL0 + normalUL9 + normalUL6 + normalUL4;

          *(vertices+1+pointerOffsetWithStride) = ll;
          *(vertices+7+pointerOffsetWithStride) = lr;
          *(vertices+13+pointerOffsetWithStride) = ur;
          *(vertices+19+pointerOffsetWithStride) = ur;
          *(vertices+25+pointerOffsetWithStride) = ul;
          *(vertices+31+pointerOffsetWithStride) = ll;

          *(vertices+3+pointerOffsetWithStride) = normalLL.x;
          *(vertices+4+pointerOffsetWithStride) = normalLL.y;
          *(vertices+5+pointerOffsetWithStride) = normalLL.z;

          *(vertices+9+pointerOffsetWithStride) = normalLR.x;
          *(vertices+10+pointerOffsetWithStride) = normalLR.y;
          *(vertices+11+pointerOffsetWithStride) = normalLR.z;

          *(vertices+15+pointerOffsetWithStride) = normalUR.x;
          *(vertices+16+pointerOffsetWithStride) = normalUR.y;
          *(vertices+17+pointerOffsetWithStride) = normalUR.z;

          *(vertices+21+pointerOffsetWithStride) = normalUR.x;
          *(vertices+22+pointerOffsetWithStride) = normalUR.y;
          *(vertices+23+pointerOffsetWithStride) = normalUR.z;

          *(vertices+27+pointerOffsetWithStride) = normalUL.x;
          *(vertices+28+pointerOffsetWithStride) = normalUL.y;
          *(vertices+29+pointerOffsetWithStride) = normalUL.z;

          *(vertices+33+pointerOffsetWithStride) = normalLL.x;
          *(vertices+34+pointerOffsetWithStride) = normalLL.y;
          *(vertices+35+pointerOffsetWithStride) = normalLL.z;
        }
      else
        {
          float ll = waterHeightOffsets[heightOffsetWithStrideForLow + tile.mapX];
          float lr = waterHeightOffsets[heightOffsetWithStrideForLow + tile.mapX + 1];
          float ur = waterHeightOffsets[heightOffsetWithStrideForUpper + tile.mapX + 1];
          float ul = waterHeightOffsets[heightOffsetWithStrideForUpper + tile.mapX];

          //just do the calculations manually (UR-LR x LL-LR and LL-UL x UR-UL)
          glm::vec3 normalLR = glm::vec3(ll - lr, 1 - glm::sqrt(glm::pow(ll - lr, 2) + glm::pow(ur - lr, 2)), ur - lr);
          glm::vec3 normalUL = glm::vec3(ul - ur, 1 - glm::sqrt(glm::pow(ul - ur, 2) + glm::pow(ul - ll, 2)), ul - ll);

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
