#include "WaterGenerator.h"

WaterGenerator::WaterGenerator(WaterShader &shaders)
  :
    Generator(),
    culledBuffers(VAO | VBO | TFBO),
    shaders(shaders)
{}

void WaterGenerator::setup()
{
  generateMap();
  while (numTiles < WORLD_WIDTH * (RIVER_WIDTH_DEFAULT + 2) * (RIVER_WIDTH_DEFAULT + 2) * 9 ||
         numTiles > WORLD_WIDTH * (RIVER_WIDTH_DEFAULT + 3) * (RIVER_WIDTH_DEFAULT + 3) * 9)
    {
      initializeMap(map);
      generateMap();
    }
}

void WaterGenerator::setupConsiderTerrain()
{
  initializeMap(postProcessMap);
  //by this moment we have smoothed shore, so make sure that water still covers it
  for (unsigned int i = 0; i < SHORE_SMOOTH_CYCLES - 1; i++)
    addWaterNearbyTerrain();
  createTiles(true, false, postProcessMap, 0);
  tiles.shrink_to_fit();
  fillBufferData();
}

void WaterGenerator::fillBufferData()
{
  numVertices = tiles.size() * UNIQUE_VERTICES_PER_TILE * WaterVertex::NUMBER_OF_ELEMENTS;
  vertices.reset(new GLfloat[numVertices]);
  const size_t INDICES_DATA_LENGTH = tiles.size() * VERTICES_PER_QUAD;
  std::unique_ptr<GLuint[]> indices(new GLuint[INDICES_DATA_LENGTH]);
  size_t indicesBufferIndex = 0;

  for (unsigned int tileIndex = 0; tileIndex < tiles.size(); tileIndex++)
    {
      TerrainTile& tile = tiles[tileIndex];
      int x = tile.mapX, y = tile.mapY;

      WaterVertex lowLeft(glm::vec3(x - 1, tile.lowLeft, y));
      WaterVertex lowRight(glm::vec3(x, tile.lowRight, y));
      WaterVertex upRight(glm::vec3(x, tile.upperRight, y - 1));
      WaterVertex upLeft(glm::vec3(x - 1, tile.upperLeft, y - 1));

      int vertexBufferOffset = tileIndex * UNIQUE_VERTICES_PER_TILE * WaterVertex::NUMBER_OF_ELEMENTS;
      bufferVertex(vertices.get(), vertexBufferOffset+0, lowLeft); //ll1
      bufferVertex(vertices.get(), vertexBufferOffset+3, lowRight); //lr1
      bufferVertex(vertices.get(), vertexBufferOffset+6, upRight); //ur1
      bufferVertex(vertices.get(), vertexBufferOffset+9, upLeft); //ur2

      GLuint indicesBufferBaseVertex = tileIndex * UNIQUE_VERTICES_PER_TILE;
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
  BufferCollection::bindZero(VAO | VBO | EBO);
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
  unsigned int curveMaxDistance = rand() % RIVER_DIRECTION_CHANGE_DELAY + RIVER_DIRECTION_CHANGE_DELAY;
  unsigned int curveDistanceStep = 0;
  int y, x;
  unsigned int startCoord = rand() % WORLD_HEIGHT;
  x = startAxisFromX ? startCoord : 0;
  y = startAxisFromX ? 0 : startCoord;
  DIRECTION riverDirection = startAxisFromX ? DOWN : RIGHT;
  int riverWidthOffset = 0, riverTileCounter = 0;
  bool riverWidthIncrease = true;

  while (!riverEnd)
    {
      switch (riverDirection) {
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
            map[y][x] = WATER_LEVEL;
            ++numTiles;
            if (curveDistanceStep == curveMaxDistance)
              setNewDirection(curveDistanceStep, curveMaxDistance, riverDirection, UP_LEFT, UP_RIGHT);
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
            map[y][x] = WATER_LEVEL;
            ++numTiles;
            if (curveDistanceStep == curveMaxDistance)
              setNewDirection(curveDistanceStep, curveMaxDistance, riverDirection, UP, RIGHT);
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
            map[y][x] = WATER_LEVEL;
            ++numTiles;
            if (curveDistanceStep == curveMaxDistance)
              setNewDirection(curveDistanceStep, curveMaxDistance, riverDirection, UP_RIGHT, DOWN_RIGHT);
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
            map[y][x] = WATER_LEVEL;
            ++numTiles;
            if (curveDistanceStep == curveMaxDistance)
              setNewDirection(curveDistanceStep, curveMaxDistance, riverDirection, DOWN, RIGHT);
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
            map[y][x] = WATER_LEVEL;
            ++numTiles;
            if (curveDistanceStep == curveMaxDistance)
              setNewDirection(curveDistanceStep, curveMaxDistance, riverDirection, DOWN_RIGHT, DOWN_LEFT);
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
            map[y][x] = WATER_LEVEL;
            ++numTiles;
            if (curveDistanceStep == curveMaxDistance)
              setNewDirection(curveDistanceStep, curveMaxDistance, riverDirection, DOWN, LEFT);
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
            map[y][x] = WATER_LEVEL;
            ++numTiles;
            if (curveDistanceStep == curveMaxDistance)
              setNewDirection(curveDistanceStep, curveMaxDistance, riverDirection, UP_LEFT, DOWN_LEFT);
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
            map[y][x] = WATER_LEVEL;
            ++numTiles;
            if (curveDistanceStep == curveMaxDistance)
              setNewDirection(curveDistanceStep, curveMaxDistance, riverDirection, UP, LEFT);
            break;
          }
        default:
          break;
      }
      fattenKernel(x, y, riverTileCounter, riverWidthOffset, riverWidthIncrease);
    }
}

void WaterGenerator::setNewDirection(unsigned int &curveDistanceStep,
                                     unsigned int &curveMaxDistance,
                                     DIRECTION &currentDirection,
                                     DIRECTION validDirectionLeft,
                                     DIRECTION validDirectionRight)
{
  curveDistanceStep = 0;
  curveMaxDistance = rand() % RIVER_DIRECTION_CHANGE_DELAY + RIVER_DIRECTION_CHANGE_DELAY;
  currentDirection = (DIRECTION)(rand() % DIRECTION::NUM_DIRECTIONS);
  while(currentDirection != validDirectionLeft && currentDirection != validDirectionRight)
    currentDirection = (DIRECTION)(rand() % DIRECTION::NUM_DIRECTIONS);
}

void WaterGenerator::fattenKernel(int x, int y, int& riverTileCounter, int& riverWidthOffset, bool& riverWidthIncrease)
{
  int shoreSizeYT = rand() % 2 + RIVER_WIDTH_DEFAULT;
  int shoreSizeYB = rand() % 2 + RIVER_WIDTH_DEFAULT;
  int shoreSizeXL = rand() % 2 + RIVER_WIDTH_DEFAULT;
  int shoreSizeXR = rand() % 2 + RIVER_WIDTH_DEFAULT;
  ++riverTileCounter;
  if (riverTileCounter % RIVER_SIZE_TO_INCREASE_COUNTER == 0)
    {
      riverTileCounter = 0;
      riverWidthOffset += riverWidthIncrease ? 1 : -1;
      if (riverWidthOffset > RIVER_WIDTH_DEFAULT)
        riverWidthIncrease = !riverWidthIncrease;
      else if (riverWidthOffset < 0)
        {
          riverWidthOffset = 0;
          riverWidthIncrease = !riverWidthIncrease;
        }
    }
  int xLeft = ((int)(x-shoreSizeXL-riverWidthOffset) <= 0 ? 0 : x-shoreSizeXL-riverWidthOffset);
  int xRight = ((int)(x+shoreSizeXR+riverWidthOffset) >= WORLD_WIDTH ? WORLD_WIDTH : x+shoreSizeXR+riverWidthOffset);
  int yTop = ((int)(y-shoreSizeYT-riverWidthOffset) <= 0 ? 0 : y-shoreSizeYT-riverWidthOffset);
  int yBottom = ((int)(y+shoreSizeYB+riverWidthOffset) >= WORLD_HEIGHT ? WORLD_HEIGHT : y+shoreSizeYB+riverWidthOffset);
  for (int y = yTop; y <= yBottom; y++)
    {
      for (int x = xLeft; x <= xRight; x++)
        {
          map[y][x] = WATER_LEVEL;
          ++numTiles;
        }
    }
}

void WaterGenerator::bufferVertex(GLfloat *vertices, int offset, WaterVertex vertex)
{
  vertices[offset+0] = vertex.posX;
  vertices[offset+1] = vertex.posY;
  vertices[offset+2] = vertex.posZ;
}

void WaterGenerator::setupGLBufferAttributes()
{
  const size_t SIZE_OF_WATER_VERTEX = WaterVertex::NUMBER_OF_ELEMENTS * sizeof(GLfloat);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, SIZE_OF_WATER_VERTEX, 0);
}

WaterGenerator::WaterVertex::WaterVertex(glm::vec3 position)
  :
    posX(position.x - HALF_WORLD_WIDTH), posY(position.y), posZ(position.z - HALF_WORLD_HEIGHT)
{}
