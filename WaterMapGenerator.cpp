#include "WaterMapGenerator.h"

WaterMapGenerator::WaterMapGenerator()
  :
    MapGenerator(){}

void WaterMapGenerator::prepareMap()
{
  unsigned int numWaterTiles = 0;
  generateMap(SHORE_SIZE_BASE, WATER_LEVEL, numWaterTiles);
  unsigned int attempts = 0;
  while (numWaterTiles < TILES_WIDTH * (SHORE_SIZE_BASE + 2) * (SHORE_SIZE_BASE + 2) * 9
         || numWaterTiles > TILES_WIDTH * (SHORE_SIZE_BASE + 3) * (SHORE_SIZE_BASE + 3) * 9)
    {
      ++attempts;
      numWaterTiles = 0;
      initializeMap(map);
      generateMap(SHORE_SIZE_BASE, WATER_LEVEL, numWaterTiles);
    }
  std::cout << "water generation attempts:\t" << attempts << std::endl;
}

void WaterMapGenerator::fillBufferData()
{
  addWaterNearbyBaseTerrain();
  fillSharpTerrainWithWater();
  createTiles(true, false);
  tiles.shrink_to_fit();
  const size_t VERTEX_DATA_LENGTH = tiles.size() * 48;
  GLfloat vertices[VERTEX_DATA_LENGTH];
  for (unsigned int i = 0; i < tiles.size(); i++)
    {
      TerrainTile& tile = tiles[i];
      int offset = i * 48;
      //ll1
      vertices[offset] = -1- TILES_WIDTH / 2 + tile.mapX;
      vertices[offset+1] = tile.lowLeft;
      vertices[offset+2] = - TILES_HEIGHT / 2 + tile.mapY;
      vertices[offset+3] = 0.0f;
      vertices[offset+4] = 0.0f;
      vertices[offset+5] = 0.0f;
      vertices[offset+6] = 1.0f;
      vertices[offset+7] = 0.0f;
      //lr1
      vertices[offset+8] = - TILES_WIDTH / 2 + tile.mapX;
      vertices[offset+9] = tile.lowRight;
      vertices[offset+10] = - TILES_HEIGHT / 2 + tile.mapY;
      vertices[offset+11] = 1.0f;
      vertices[offset+12] = 0.0f;
      vertices[offset+13] = 0.0f;
      vertices[offset+14] = 1.0f;
      vertices[offset+15] = 0.0f;
      //ur1
      vertices[offset+16] = - TILES_WIDTH / 2 + tile.mapX;
      vertices[offset+17] = tile.upperRight;
      vertices[offset+18] = -1 - TILES_HEIGHT / 2 + tile.mapY;
      vertices[offset+19] = 1.0f;
      vertices[offset+20] = 1.0f;
      vertices[offset+21] = 0.0f;
      vertices[offset+22] = 1.0f;
      vertices[offset+23] = 0.0f;
      //ur2
      vertices[offset+24] = - TILES_WIDTH / 2 + tile.mapX;
      vertices[offset+25] = tile.upperRight;
      vertices[offset+26] = -1 - TILES_HEIGHT / 2 + tile.mapY;
      vertices[offset+27] = 1.0f;
      vertices[offset+28] = 1.0f;
      vertices[offset+29] = 0.0f;
      vertices[offset+30] = 1.0f;
      vertices[offset+31] = 0.0f;
      //ul2
      vertices[offset+32] = -1 - TILES_WIDTH / 2 + tile.mapX;
      vertices[offset+33] = tile.upperLeft;
      vertices[offset+34] = -1 - TILES_HEIGHT / 2 + tile.mapY;
      vertices[offset+35] = 0.0f;
      vertices[offset+36] = 1.0f;
      vertices[offset+37] = 0.0f;
      vertices[offset+38] = 1.0f;
      vertices[offset+39] = 0.0f;
      //ll2
      vertices[offset+40] = -1- TILES_WIDTH / 2 + tile.mapX;
      vertices[offset+41] = tile.lowLeft;
      vertices[offset+42] = - TILES_HEIGHT / 2 + tile.mapY;
      vertices[offset+43] = 0.0f;
      vertices[offset+44] = 0.0f;
      vertices[offset+45] = 0.0f;
      vertices[offset+46] = 1.0f;
      vertices[offset+47] = 0.0f;
    }
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
  resetAllGLBuffers();
}

void WaterMapGenerator::addWaterNearbyBaseTerrain()
{
  //add water above the tile
  for (unsigned int y = 0; y < TILES_HEIGHT - 1; y++)
    {
      for (unsigned int x = 0; x < TILES_WIDTH; x++)
        {
          if (map[y+1][x] != 0)
            map[y][x] = map[y+1][x];
        }
    }
  //add water below the tile
  for (unsigned int y = TILES_HEIGHT; y > 0; y--)
    {
      for (unsigned int x = 0; x < TILES_WIDTH; x++)
        {
          if (map[y-1][x] != 0)
            map[y][x] = map[y-1][x];
        }
    }
  //add water left to the tile
  for (unsigned int x = 0; x < TILES_WIDTH - 1; x++)
    {
      for (unsigned int y = 0; y < TILES_HEIGHT; y++)
        {
          if (map[y][x+1] != 0)
            map[y][x] = map[y][x+1];
        }
    }
  //add water right to the tile
  for (unsigned int x = TILES_WIDTH; x > 0; x--)
    {
      for (unsigned int y = 0; y < TILES_HEIGHT; y++)
        {
          if (map[y][x-1] != 0)
            map[y][x] = map[y][x-1];
        }
    }
}

void WaterMapGenerator::fillSharpTerrainWithWater()
{
  for (int y2 = 1; y2 < TILES_HEIGHT - 1; y2++)
    {
      for (int x2 = 1; x2 < TILES_WIDTH - 1; x2++)
        {
          if (map[y2][x2] == WATER_LEVEL)
            continue;
          if ((map[y2-1][x2] == WATER_LEVEL && map[y2+1][x2] == WATER_LEVEL) || (map[y2][x2-1] == WATER_LEVEL && map[y2][x2+1] == WATER_LEVEL))
            map[y2][x2] = WATER_LEVEL;
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

void WaterMapGenerator::generateMap(unsigned int shoreSizeBase, float waterLevel, unsigned int &numWaterTiles)
{
  srand(time(NULL));
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
