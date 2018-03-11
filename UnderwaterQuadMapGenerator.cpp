#include "UnderwaterQuadMapGenerator.h"

UnderwaterQuadMapGenerator::UnderwaterQuadMapGenerator()
  :
    MapGenerator(){}

void UnderwaterQuadMapGenerator::fillQuadBufferData()
{
  GLfloat vertices[20] = {
    -TILES_WIDTH / 2.0f, UNDERWATER_BASE_TILE_HEIGHT, TILES_HEIGHT / 2.0f, 0.0f, 0.0f,
     TILES_WIDTH / 2.0f, UNDERWATER_BASE_TILE_HEIGHT, TILES_HEIGHT / 2.0f, TILES_WIDTH, 0.0f,
     TILES_WIDTH / 2.0f, UNDERWATER_BASE_TILE_HEIGHT,-TILES_HEIGHT / 2.0f, TILES_WIDTH, TILES_HEIGHT,
    -TILES_WIDTH / 2.0f, UNDERWATER_BASE_TILE_HEIGHT,-TILES_HEIGHT / 2.0f, 0.0f, TILES_HEIGHT
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

void UnderwaterQuadMapGenerator::draw(GLuint& underwaterTexture)
{
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, underwaterTexture);
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void UnderwaterQuadMapGenerator::deleteGLObjects()
{
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
}

GLuint &UnderwaterQuadMapGenerator::getVAO()
{
  return vao;
}

GLuint &UnderwaterQuadMapGenerator::getVBO()
{
  return vbo;
}

GLuint &UnderwaterQuadMapGenerator::getEBO()
{
  return ebo;
}
