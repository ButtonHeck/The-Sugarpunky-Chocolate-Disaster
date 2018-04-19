#include "BuildableMapGenerator.h"

BuildableMapGenerator::BuildableMapGenerator(std::vector<std::vector<float> > &baseMap, std::vector<std::vector<float> > &hillMap)
  :
    MapGenerator(),
    baseMap(baseMap),
    hillMap(hillMap)
{

}

void BuildableMapGenerator::prepareMap()
{
  for (unsigned int y = 1; y < TILES_HEIGHT - 1; y++)
    {
      for (unsigned int x = 1; x < TILES_WIDTH - 1; x++)
        {
          if (baseMap[y][x] == 0
              && baseMap[y+1][x] == 0
              && baseMap[y+1][x+1] == 0
              && baseMap[y][x+1] == 0
              && hillMap[y][x] == 0
              && hillMap[y+1][x] == 0
              && hillMap[y+1][x+1] == 0
              && hillMap[y][x+1] == 0)
            {
              map[y+1][x+1] = 0.01f;
            }
        }
    }
  createTiles(true, false, map);
  tiles.shrink_to_fit();
}

void BuildableMapGenerator::fillBufferData()
{
  const size_t VERTEX_DATA_LENGTH = tiles.size() * 18;
  GLfloat vertices[VERTEX_DATA_LENGTH];
  for (unsigned int i = 0; i < tiles.size(); i++)
    {
      TerrainTile& tile = tiles[i];
      int offset = i * 18;
      //ll1
      vertices[offset] = -0.9f- TILES_WIDTH / 2 + tile.mapX;
      vertices[offset+1] = tile.lowLeft;
      vertices[offset+2] = -0.1f- TILES_HEIGHT / 2 + tile.mapY;
      //lr1
      vertices[offset+3] = -0.1f- TILES_WIDTH / 2 + tile.mapX;
      vertices[offset+4] = tile.lowRight;
      vertices[offset+5] = -0.1f- TILES_HEIGHT / 2 + tile.mapY;
      //ur1
      vertices[offset+6] = -0.1f- TILES_WIDTH / 2 + tile.mapX;
      vertices[offset+7] = tile.upperRight;
      vertices[offset+8] = -0.9f - TILES_HEIGHT / 2 + tile.mapY;
      //ur2
      vertices[offset+9] = -0.1f- TILES_WIDTH / 2 + tile.mapX;
      vertices[offset+10] = tile.upperRight;
      vertices[offset+11] = -0.9f - TILES_HEIGHT / 2 + tile.mapY;
      //ul2
      vertices[offset+12] = -0.9f - TILES_WIDTH / 2 + tile.mapX;
      vertices[offset+13] = tile.upperLeft;
      vertices[offset+14] = -0.9f - TILES_HEIGHT / 2 + tile.mapY;
      //ll2
      vertices[offset+15] = -0.9f- TILES_WIDTH / 2 + tile.mapX;
      vertices[offset+16] = tile.lowLeft;
      vertices[offset+17] = -0.1f- TILES_HEIGHT / 2 + tile.mapY;
    }
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  resetAllGLBuffers();
}
