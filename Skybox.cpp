#include "Skybox.h"

Skybox::Skybox(const std::string &directory, TextureLoader& loader, GLuint textureUnit)
  :
    directory(directory),
    loader(loader),
    textureUnit(textureUnit)
{
  faces.assign(
  {
    directory + "right.png",
          directory + "left.png",
          directory + "up.png",
          directory + "down.png",
          directory + "back.png",
          directory + "front.png"
        });
  texture = loader.loadCubemap(faces, textureUnit);
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * NUM_POSITIONS, &positions[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Skybox::deleteGLObjects()
{
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
}

GLuint &Skybox::getVAO()
{
  return vao;
}

GLuint &Skybox::getTexture()
{
  return texture;
}
