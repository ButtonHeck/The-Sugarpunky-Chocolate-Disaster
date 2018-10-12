#include "generator/UnderwaterGenerator.h"

UnderwaterGenerator::UnderwaterGenerator()
  :
    Generator()
{
  constexpr GLfloat VERTICES[20] = {
    -(float)HALF_WORLD_WIDTH, UNDERWATER_TILE_YPOS, (float)HALF_WORLD_HEIGHT, 0.0f, 0.0f,
     (float)HALF_WORLD_WIDTH, UNDERWATER_TILE_YPOS, (float)HALF_WORLD_HEIGHT, WORLD_WIDTH, 0.0f,
     (float)HALF_WORLD_WIDTH, UNDERWATER_TILE_YPOS,-(float)HALF_WORLD_HEIGHT, WORLD_WIDTH, WORLD_HEIGHT,
    -(float)HALF_WORLD_WIDTH, UNDERWATER_TILE_YPOS,-(float)HALF_WORLD_HEIGHT, 0.0f, WORLD_HEIGHT
  };
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QUAD_INDICES), QUAD_INDICES, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  resetAllGLBuffers();
}
