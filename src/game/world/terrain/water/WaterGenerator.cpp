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

      bufferVertex(vertices.get(), offset+0, lowLeft); //ll1
      bufferVertex(vertices.get(), offset+6, lowRight); //lr1
      bufferVertex(vertices.get(), offset+12, upRight); //ur1
      bufferVertex(vertices.get(), offset+18, upRight); //ur2
      bufferVertex(vertices.get(), offset+24, upLeft); //ul2
      bufferVertex(vertices.get(), offset+30, lowLeft); //ll2
    }
  basicGLBuffers.bind(VAO | VBO);
  glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(GLfloat), vertices.get(), GL_STREAM_DRAW);
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
  constexpr float NORMAL_Y_APPROX = 0.7f; //fake "true" normal calculation (use no sqrt and pow)
  using glm::vec3;
  using glm::vec4;
  double offsetMultiplier = time * 0.12;
  for (size_t i = 0; i < WATER_HEIGHT_OFFSETS_SIZE; i+=2)
    {
      heightOffsets[i] = std::cos(offsetMultiplier * ((i * i) % 19)) * 0.0825 + WATER_LEVEL;
      heightOffsets[i+1] = std::sin(offsetMultiplier * ((i * i) % 29)) * 0.0889 + WATER_LEVEL;
    }
  unsigned int numWaterTiles = tiles.size();
  for (unsigned int i = 0; i < numWaterTiles; ++i)
    {
      if (options[OPT_RECREATE_TERRAIN_REQUEST])
        return;
      TerrainTile& tile = tiles[i];
      unsigned int pointerOffsetWithStride = i * 36;
      unsigned int heightOffsetWithStrideForLow = (tile.mapY+1) * WORLD_WIDTH;
      unsigned int heightOffsetWithStrideForUpper = tile.mapY * WORLD_WIDTH;

      float ll = heightOffsets[heightOffsetWithStrideForLow + tile.mapX];
      float lr = heightOffsets[heightOffsetWithStrideForLow + tile.mapX + 1];
      float ur = heightOffsets[heightOffsetWithStrideForUpper + tile.mapX + 1];
      float ul = heightOffsets[heightOffsetWithStrideForUpper + tile.mapX];
      updateTileY(vertices.get(), pointerOffsetWithStride, vec4(ll, lr, ur, ul));

      float ll_xm1 = heightOffsets[heightOffsetWithStrideForLow + tile.mapX - 1];
      float ll_yp1 = heightOffsets[heightOffsetWithStrideForLow + tile.mapX + WORLD_WIDTH];
      float ll_xm1_yp1 = heightOffsets[heightOffsetWithStrideForLow + tile.mapX + WORLD_WIDTH - 1];
      float lr_yp1 = heightOffsets[heightOffsetWithStrideForLow + tile.mapX + WORLD_WIDTH + 1];
      float lr_xp1 = heightOffsets[heightOffsetWithStrideForLow + tile.mapX + 2];
      float ur_xp1 = heightOffsets[heightOffsetWithStrideForUpper + tile.mapX + 2];
      float ur_ym1 = heightOffsets[heightOffsetWithStrideForUpper + tile.mapX - WORLD_WIDTH + 1];
      float ur_xp1_ym1 = heightOffsets[heightOffsetWithStrideForUpper + tile.mapX - WORLD_WIDTH + 2];
      float ul_ym1 = heightOffsets[heightOffsetWithStrideForUpper + tile.mapX - WORLD_WIDTH];
      float ul_xm1 = heightOffsets[heightOffsetWithStrideForUpper + tile.mapX - 1];

      vec3 normalLL3 =  vec3(ll - lr, NORMAL_Y_APPROX, ur - lr);
      vec3 normalLL12 = vec3(ul - ur, NORMAL_Y_APPROX, ul - ll);
      vec3 normalLL0 =  vec3(ll_xm1 - ll, NORMAL_Y_APPROX, ul - ll);
      vec3 normalLL9 =  vec3(ll_xm1 - ll, NORMAL_Y_APPROX, ll_xm1 - ll_xm1_yp1);
      vec3 normalLL6 =  vec3(ll_xm1_yp1 - ll_yp1, NORMAL_Y_APPROX, ll - ll_yp1);
      vec3 normalLL4 =  vec3(ll - lr, NORMAL_Y_APPROX, ll - ll_yp1);
      vec3 normalLL = normalLL3 + normalLL12 + normalLL0 + normalLL9 + normalLL6 + normalLL4;

      vec3 normalLR3 =  vec3(lr - lr_xp1, NORMAL_Y_APPROX, ur_xp1 - lr_xp1);
      vec3 normalLR12 = vec3(ur - ur_xp1, NORMAL_Y_APPROX, ur - lr);
      vec3 normalLR0 = normalLL3;
      vec3 normalLR9 = normalLL4;
      vec3 normalLR6 =  vec3(ll_yp1 - lr_yp1, NORMAL_Y_APPROX, lr - lr_yp1);
      vec3 normalLR4 =  vec3(lr - lr_xp1, NORMAL_Y_APPROX, lr - lr_xp1);
      vec3 normalLR = normalLR3 + normalLR12 + normalLR0 + normalLR9 + normalLR6 + normalLR4;

      vec3 normalUR3 =  vec3(ur - ur_xp1, NORMAL_Y_APPROX, ur_xp1_ym1 - ur_xp1);
      vec3 normalUR12 = vec3(ur_ym1 - ur_xp1_ym1, NORMAL_Y_APPROX, ur_ym1 - ur);
      vec3 normalUR0 =  vec3(ul - ur, NORMAL_Y_APPROX, ur_ym1 - ur);
      vec3 normalUR9 = normalLL12;
      vec3 normalUR6 = normalLL3;
      vec3 normalUR4 = normalLR12;
      vec3 normalUR = normalUR3 + normalUR12 + normalUR0 + normalUR9 + normalUR6 + normalUR4;

      vec3 normalUL3 = normalUR0;
      vec3 normalUL12 = vec3(ul_ym1 - ur_ym1, NORMAL_Y_APPROX, ul_ym1 - ul);
      vec3 normalUL0 =  vec3(ul_xm1 - ul, NORMAL_Y_APPROX, ul_ym1 - ul);
      vec3 normalUL9 =  vec3(ul_xm1 - ul, NORMAL_Y_APPROX, ul_xm1 - ll_xm1);
      vec3 normalUL6 = normalLL0;
      vec3 normalUL4 = normalLL12;
      vec3 normalUL = normalUL3 + normalUL12 + normalUL0 + normalUL9 + normalUL6 + normalUL4;

      updateVertexNormal(vertices.get(), pointerOffsetWithStride+3, normalLL);
      updateVertexNormal(vertices.get(), pointerOffsetWithStride+9, normalLR);
      updateVertexNormal(vertices.get(), pointerOffsetWithStride+15, normalUR);
      updateVertexNormal(vertices.get(), pointerOffsetWithStride+21, normalUR);
      updateVertexNormal(vertices.get(), pointerOffsetWithStride+27, normalUL);
      updateVertexNormal(vertices.get(), pointerOffsetWithStride+33, normalLL);
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

void WaterGenerator::updateVertexNormal(GLfloat *vertices, int offset, glm::vec3 normal)
{
  vertices[offset+0] = normal.x;
  vertices[offset+1] = normal.y;
  vertices[offset+2] = normal.z;
}

void WaterGenerator::updateTileY(GLfloat *vertices, int offset, glm::vec4 heights)
{
  vertices[offset+1] = heights.x;
  vertices[offset+7] = heights.y;
  vertices[offset+13] = heights.z;
  vertices[offset+19] = heights.z;
  vertices[offset+25] = heights.w;
  vertices[offset+31] = heights.x;
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
