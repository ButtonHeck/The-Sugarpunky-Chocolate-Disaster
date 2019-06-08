#include "UnderwaterGenerator"

UnderwaterGenerator::UnderwaterGenerator()
  :
    Generator()
{
  constexpr GLfloat VERTICES[20] = {
    -HALF_WORLD_WIDTH_F, UNDERWATER_TILE_YPOS, HALF_WORLD_HEIGHT_F, 0.0f,        0.0f,
     HALF_WORLD_WIDTH_F, UNDERWATER_TILE_YPOS, HALF_WORLD_HEIGHT_F, WORLD_WIDTH, 0.0f,
     HALF_WORLD_WIDTH_F, UNDERWATER_TILE_YPOS,-HALF_WORLD_HEIGHT_F, WORLD_WIDTH, WORLD_HEIGHT,
    -HALF_WORLD_WIDTH_F, UNDERWATER_TILE_YPOS,-HALF_WORLD_HEIGHT_F, 0.0f,        WORLD_HEIGHT
  };
  basicGLBuffers.bind(VAO | VBO | EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QUAD_INDICES), QUAD_INDICES, GL_STATIC_DRAW);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  BufferCollection::bindZero(VAO | VBO | EBO);
}
