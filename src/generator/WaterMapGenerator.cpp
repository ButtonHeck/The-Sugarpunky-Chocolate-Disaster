#include "generator/WaterMapGenerator.h"

WaterMapGenerator::WaterMapGenerator(Shader &waterShader)
  :
    MapGenerator(),
    waterShader(waterShader),
    waterHeightOffsets(new GLfloat[WATER_HEIGHT_OFFSETS_SIZE])
{}

WaterMapGenerator::~WaterMapGenerator()
{
  glDeleteVertexArrays(1, &culledVAO);
  glDeleteBuffers(1, &culledVBO);
  glDeleteTransformFeedbacks(1, &TFBO);
}

void WaterMapGenerator::setup()
{
  unsigned int numWaterTiles = 0;
  generateMap(SHORE_SIZE_BASE, numWaterTiles);
  while (numWaterTiles < WORLD_WIDTH * (SHORE_SIZE_BASE + 2) * (SHORE_SIZE_BASE + 2) * 9
         || numWaterTiles > WORLD_WIDTH * (SHORE_SIZE_BASE + 3) * (SHORE_SIZE_BASE + 3) * 9)
    {
      numWaterTiles = 0;
      initializeMap(map);
      generateMap(SHORE_SIZE_BASE, numWaterTiles);
    }
}

void WaterMapGenerator::setupConsiderTerrain()
{
  initializeMap(postProcessMap);
  for (unsigned int i = 0; i < 4; i++)
    addWaterNearbyTerrain();
  createTiles(true, false, postProcessMap, 0);
  tiles.shrink_to_fit();
}

void WaterMapGenerator::fillBufferData()
{
  numVertices = tiles.size() * 36;
  vertices.reset(new GLfloat[numVertices]);
  const static glm::vec3 DEFAULT_Y_NORMAL(0.0f, 1.0f, 0.0f);

  for (unsigned int i = 0; i < tiles.size(); i++)
    {
      TerrainTile& tile = tiles[i];
      int offset = i * 36;
      WaterVertex lowLeft(glm::vec3(tile.mapX - 1, tile.lowLeft, tile.mapY), DEFAULT_Y_NORMAL);
      WaterVertex lowRight(glm::vec3(tile.mapX, tile.lowRight, tile.mapY), DEFAULT_Y_NORMAL);
      WaterVertex upRight(glm::vec3(tile.mapX, tile.upperRight, tile.mapY - 1), DEFAULT_Y_NORMAL);
      WaterVertex upLeft(glm::vec3(tile.mapX - 1, tile.upperLeft, tile.mapY - 1), DEFAULT_Y_NORMAL);

      bufferVertex(vertices.get(), offset, lowLeft); //ll1
      bufferVertex(vertices.get(), offset+6, lowRight); //lr1
      bufferVertex(vertices.get(), offset+12, upRight); //ur1
      bufferVertex(vertices.get(), offset+18, upRight); //ur2
      bufferVertex(vertices.get(), offset+24, upLeft); //ul2
      bufferVertex(vertices.get(), offset+30, lowLeft); //ll2
    }
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(GLfloat), vertices.get(), GL_STREAM_DRAW);
  setupGLBufferAttributes();

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
  setupGLBufferAttributes();
  glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, TFBO);
  const GLchar* varyings[2] = {"o_pos", "o_normal"};
  glTransformFeedbackVaryings(waterShader.getID(), 2, varyings, GL_INTERLEAVED_ATTRIBS);
  waterShader.linkAgain();
  glTransformFeedbackBufferBase(TFBO, 0, culledVBO);

  resetAllGLBuffers();
}

void WaterMapGenerator::updateVerticesBuffer()
{
  glNamedBufferData(vbo, numVertices * sizeof(GLfloat), vertices.get(), GL_STREAM_DRAW);
}

void WaterMapGenerator::addWaterNearbyTerrain()
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

void WaterMapGenerator::generateMap(int shoreSizeBase, unsigned int &numTiles)
{
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
            setNewDirection(x, y, numTiles, numCurveChanges, curveDistanceStep, curveMaxDistance, dir, UP_LEFT, UP_RIGHT);
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
            setNewDirection(x, y, numTiles, numCurveChanges, curveDistanceStep, curveMaxDistance, dir, UP, RIGHT);
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
            setNewDirection(x, y, numTiles, numCurveChanges, curveDistanceStep, curveMaxDistance, dir, UP_RIGHT, DOWN_RIGHT);
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
            setNewDirection(x, y, numTiles, numCurveChanges, curveDistanceStep, curveMaxDistance, dir, DOWN, RIGHT);
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
            setNewDirection(x, y, numTiles, numCurveChanges, curveDistanceStep, curveMaxDistance, dir, DOWN_RIGHT, DOWN_LEFT);
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
            setNewDirection(x, y, numTiles, numCurveChanges, curveDistanceStep, curveMaxDistance, dir, DOWN, LEFT);
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
            setNewDirection(x, y, numTiles, numCurveChanges, curveDistanceStep, curveMaxDistance, dir, UP_LEFT, DOWN_LEFT);
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
            setNewDirection(x, y, numTiles, numCurveChanges, curveDistanceStep, curveMaxDistance, dir, UP, LEFT);
            break;
          }
        default:
          break;
      }
      fattenKernel(x, y, shoreSizeBase, numTiles, riverTileCounter, shoreSizeOffset, shoreSizeIncrease);
    }
}

void WaterMapGenerator::setNewDirection(int x, int y,
                                 unsigned int& numTiles,
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

void WaterMapGenerator::fattenKernel(int x, int y, int shoreSizeBase, unsigned int& numTiles,
                                     int& riverTileCounter, int& shoreSizeOffset, bool& shoreSizeIncrease)
{
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

void WaterMapGenerator::updateAnimationFrame(Options& options)
{
  BENCHMARK("(SI/ST)Water: Update animation frame", true);
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
      if (options.get(OPT_RECREATE_TERRAIN_REQUEST))
        return;
      TerrainTile& tile = tiles[i];
      unsigned int pointerOffsetWithStride = i * 36;
      unsigned int heightOffsetWithStrideForLow = (tile.mapY+1) * WORLD_WIDTH;
      unsigned int heightOffsetWithStrideForUpper = tile.mapY * WORLD_WIDTH;

      if(tile.mapX > 0 && tile.mapX + 2 < WORLD_WIDTH && tile.mapY - 2 > 0 && tile.mapY + 1 < WORLD_HEIGHT)
        {
          float ll = waterHeightOffsets[heightOffsetWithStrideForLow + tile.mapX];
          float lr = waterHeightOffsets[heightOffsetWithStrideForLow + tile.mapX + 1];
          float ur = waterHeightOffsets[heightOffsetWithStrideForUpper + tile.mapX + 1];
          float ul = waterHeightOffsets[heightOffsetWithStrideForUpper + tile.mapX];
          float ll_xm1 = waterHeightOffsets[heightOffsetWithStrideForLow + tile.mapX - 1];
          float ll_yp1 = waterHeightOffsets[heightOffsetWithStrideForLow + tile.mapX + WORLD_WIDTH];
          float ll_xm1_yp1 = waterHeightOffsets[heightOffsetWithStrideForLow + tile.mapX + WORLD_WIDTH - 1];
          float lr_yp1 = waterHeightOffsets[heightOffsetWithStrideForLow + tile.mapX + WORLD_WIDTH + 1];
          float lr_xp1 = waterHeightOffsets[heightOffsetWithStrideForLow + tile.mapX + 2];
          float ur_xp1 = waterHeightOffsets[heightOffsetWithStrideForUpper + tile.mapX + 2];
          float ur_ym1 = waterHeightOffsets[heightOffsetWithStrideForUpper + tile.mapX - WORLD_WIDTH + 1];
          float ur_xp1_ym1 = waterHeightOffsets[heightOffsetWithStrideForUpper + tile.mapX - WORLD_WIDTH + 2];
          float ul_ym1 = waterHeightOffsets[heightOffsetWithStrideForUpper + tile.mapX - WORLD_WIDTH];
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

          vertices[1+pointerOffsetWithStride] = ll;
          vertices[7+pointerOffsetWithStride] = lr;
          vertices[13+pointerOffsetWithStride] = ur;
          vertices[19+pointerOffsetWithStride] = ur;
          vertices[25+pointerOffsetWithStride] = ul;
          vertices[31+pointerOffsetWithStride] = ll;

          vertices[3+pointerOffsetWithStride] = normalLL.x;
          vertices[4+pointerOffsetWithStride] = normalLL.y;
          vertices[5+pointerOffsetWithStride] = normalLL.z;

          vertices[9+pointerOffsetWithStride] = normalLR.x;
          vertices[10+pointerOffsetWithStride] = normalLR.y;
          vertices[11+pointerOffsetWithStride] = normalLR.z;

          vertices[15+pointerOffsetWithStride] = normalUR.x;
          vertices[16+pointerOffsetWithStride] = normalUR.y;
          vertices[17+pointerOffsetWithStride] = normalUR.z;

          vertices[21+pointerOffsetWithStride] = normalUR.x;
          vertices[22+pointerOffsetWithStride] = normalUR.y;
          vertices[23+pointerOffsetWithStride] = normalUR.z;

          vertices[27+pointerOffsetWithStride] = normalUL.x;
          vertices[28+pointerOffsetWithStride] = normalUL.y;
          vertices[29+pointerOffsetWithStride] = normalUL.z;

          vertices[33+pointerOffsetWithStride] = normalLL.x;
          vertices[34+pointerOffsetWithStride] = normalLL.y;
          vertices[35+pointerOffsetWithStride] = normalLL.z;
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

          vertices[1+pointerOffsetWithStride] = ll;
          vertices[7+pointerOffsetWithStride] = lr;
          vertices[13+pointerOffsetWithStride] = ur;
          vertices[19+pointerOffsetWithStride] = ur;
          vertices[25+pointerOffsetWithStride] = ul;
          vertices[31+pointerOffsetWithStride] = ll;

          vertices[3+pointerOffsetWithStride] = normalLR.x;
          vertices[5+pointerOffsetWithStride] = normalLR.z;
          vertices[9+pointerOffsetWithStride] = normalLR.x;
          vertices[11+pointerOffsetWithStride] = normalLR.z;
          vertices[15+pointerOffsetWithStride] = normalLR.x;
          vertices[17+pointerOffsetWithStride] = normalLR.z;

          vertices[21+pointerOffsetWithStride] = normalUL.x;
          vertices[23+pointerOffsetWithStride] = normalUL.z;
          vertices[27+pointerOffsetWithStride] = normalUL.x;
          vertices[29+pointerOffsetWithStride] = normalUL.z;
          vertices[33+pointerOffsetWithStride] = normalUL.x;
          vertices[35+pointerOffsetWithStride] = normalUL.z;
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

void WaterMapGenerator::bufferVertex(GLfloat *vertices, int offset, WaterMapGenerator::WaterVertex vertex)
{
  vertices[offset] = vertex.posX;
  vertices[offset+1] = vertex.posY;
  vertices[offset+2] = vertex.posZ;
  vertices[offset+3] = vertex.normalX;
  vertices[offset+4] = vertex.normalY;
  vertices[offset+5] = vertex.normalZ;
}

void WaterMapGenerator::setupGLBufferAttributes()
{
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
}

WaterMapGenerator::WaterVertex::WaterVertex(glm::vec3 position, glm::vec3 normal)
  :
    posX(position.x - HALF_WORLD_WIDTH), posY(position.y), posZ(position.z - HALF_WORLD_HEIGHT),
    normalX(normal.x), normalY(normal.y), normalZ(normal.z)
{}
