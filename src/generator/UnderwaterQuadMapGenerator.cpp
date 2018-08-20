#include "generator/UnderwaterQuadMapGenerator.h"

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
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QUAD_INDICES), QUAD_INDICES, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  resetAllGLBuffers();
}

UnderwaterQuadMapGenerator::~UnderwaterQuadMapGenerator()
{
  deleteGLObjects();
}
