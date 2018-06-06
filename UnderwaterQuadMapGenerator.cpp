#include "UnderwaterQuadMapGenerator.h"

UnderwaterQuadMapGenerator::UnderwaterQuadMapGenerator()
  :
    MapGenerator()
{
  GLfloat vertices[20] = {
    -(float)HALF_TILES_WIDTH, UNDERWATER_BASE_TILE_HEIGHT, (float)HALF_TILES_HEIGHT, 0.0f, 0.0f,
     (float)HALF_TILES_WIDTH, UNDERWATER_BASE_TILE_HEIGHT, (float)HALF_TILES_HEIGHT, TILES_WIDTH, 0.0f,
     (float)HALF_TILES_WIDTH, UNDERWATER_BASE_TILE_HEIGHT,-(float)HALF_TILES_HEIGHT, TILES_WIDTH, TILES_HEIGHT,
    -(float)HALF_TILES_WIDTH, UNDERWATER_BASE_TILE_HEIGHT,-(float)HALF_TILES_HEIGHT, 0.0f, TILES_HEIGHT
  };
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QUAD_INDICES), QUAD_INDICES, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  setupGLBuffersAttributes();
  resetAllGLBuffers();
}
