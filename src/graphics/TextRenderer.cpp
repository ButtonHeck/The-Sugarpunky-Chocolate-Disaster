#include "TextRenderer.h"

TextRenderer::TextRenderer(FontLoader &font, Shader &shader)
  :
    font(font),
    shader(shader),
    vertexData(new GLfloat[MAX_BUFFER_SIZE])
{
  glCreateVertexArrays(1, &vao);
  glCreateBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  glBindVertexArray(0);
}

void TextRenderer::addText(std::string text, GLfloat x, GLfloat y, GLfloat scale)
{
  std::string::const_iterator c;
  GLfloat xpos = x;
  GLfloat ypos = y;
  const std::map<char, Character>& alphabet = font.getAlphabet();
  float textureWidth = (float)font.getTextureWidth(), textureHeight = (float)font.getTextureHeight();
  for (c = text.begin(); c != text.end(); ++c)
    {
      Character ch = alphabet.at(*c);
      //vertex1 pos
      vertexData[bufferOffset++] = xpos + ch.xoffset * scale;
      vertexData[bufferOffset++] = ypos - ch.yoffset * scale;
      //vertex1 texCoords
      vertexData[bufferOffset++] = (float)ch.x / textureWidth;
      vertexData[bufferOffset++] = (float)ch.y / textureHeight;
      //vertex2 pos
      vertexData[bufferOffset++] = xpos + (ch.width + ch.xoffset) * scale;
      vertexData[bufferOffset++] = ypos - ch.yoffset * scale;
      //vertex2 texCoords
      vertexData[bufferOffset++] = (float)(ch.x + ch.width) / textureWidth;
      vertexData[bufferOffset++] = (float)ch.y / textureHeight;
      //vertex3 pos
      vertexData[bufferOffset++] = xpos + (ch.width + ch.xoffset) * scale;
      vertexData[bufferOffset++] = ypos - (ch.height + ch.yoffset) * scale;
      //vertex3 texCoords
      vertexData[bufferOffset++] = (float)(ch.x + ch.width) / textureWidth;
      vertexData[bufferOffset++] = (float)(ch.y + ch.height) / textureHeight;
      //vertex4 pos
      vertexData[bufferOffset++] = xpos + (ch.width + ch.xoffset) * scale;
      vertexData[bufferOffset++] = ypos - (ch.height + ch.yoffset) * scale;
      //vertex4 texCoords
      vertexData[bufferOffset++] = (float)(ch.x + ch.width) / textureWidth;
      vertexData[bufferOffset++] = (float)(ch.y + ch.height) / textureHeight;
      //vertex5 pos
      vertexData[bufferOffset++] = xpos + ch.xoffset * scale;
      vertexData[bufferOffset++] = ypos - (ch.height + ch.yoffset) * scale;
      //vertex5 texCoords
      vertexData[bufferOffset++] = (float)ch.x / textureWidth;
      vertexData[bufferOffset++] = (float)(ch.y + ch.height) / textureHeight;
      //vertex6 pos
      vertexData[bufferOffset++] = xpos + ch.xoffset * scale;
      vertexData[bufferOffset++] = ypos - ch.yoffset * scale;
      //vertex6 texCoords
      vertexData[bufferOffset++] = (float)ch.x / textureWidth;
      vertexData[bufferOffset++] = (float)ch.y / textureHeight;

      xpos += ch.xadvance * scale;
      ++vertexCount;
    }
}

void TextRenderer::drawText()
{
  shader.use();
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, bufferOffset * sizeof(GLfloat), vertexData.get(), GL_STATIC_DRAW);
  glDrawArrays(GL_TRIANGLES, 0, vertexCount * VERTICES_PER_TILE);
  bufferOffset = 0;
  vertexCount = 0;
}
