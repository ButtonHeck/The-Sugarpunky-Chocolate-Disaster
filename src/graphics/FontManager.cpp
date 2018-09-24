#include "FontManager.h"

FontManager::FontManager(const std::string &fontFile, const std::string &textureFile, glm::mat4 projection, Shader& shader)
  :
    fontShader(shader)
{
  if (!ilLoadImage(textureFile.c_str()))
    std::cerr << "Error loading font texture " << textureFile.c_str() << std::endl;
  ILubyte* data = ilGetData();
  auto width = ilGetInteger(IL_IMAGE_WIDTH);
  textureWidth = width;
  auto height = ilGetInteger(IL_IMAGE_HEIGHT);
  textureHeight = height;
  glCreateTextures(GL_TEXTURE_2D, 1, &fontTexture);
  glActiveTexture(GL_TEXTURE0 + TEX_FONT);
  glBindTexture(GL_TEXTURE_2D, fontTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  std::ifstream input(fontFile);
  std::stringstream stringBuf;
  stringBuf << input.rdbuf();
  for (unsigned int i = 0; i < 24; i++)
    {
      std::string unused;
      stringBuf >> unused;
    }

  for (unsigned int i = 0; i < 97; ++i)
    {
      Character c;
      std::string token;
      stringBuf >> token; //char
      stringBuf >> token; //id=
      token = token.substr(3);
      int code = std::stoi(token);
      c.asciiCode = code;
      stringBuf >> token; //x=
      token = token.substr(2);
      int x = std::stoi(token);
      c.x = x;
      stringBuf >> token; //y=
      token = token.substr(2);
      int y = std::stoi(token);
      c.y = y;
      stringBuf >> token; //width=
      token = token.substr(6);
      int width = std::stoi(token);
      c.width = width;
      stringBuf >> token; //height=
      token = token.substr(7);
      int height = std::stoi(token);
      c.height = height;
      stringBuf >> token; //xoffset=
      token = token.substr(8);
      int xoffset = std::stoi(token);
      c.xoffset = xoffset;
      stringBuf >> token; //yoffset=
      token = token.substr(8);
      int yoffset = std::stoi(token);
      c.yoffset = yoffset;
      stringBuf >> token; //xadvance=
      token = token.substr(9);
      int xadvance = std::stoi(token);
      c.xadvance = xadvance;
      stringBuf >> token; //page=0
      stringBuf >> token; //chnl=0
      chars[code] = c;
    }
  input.close();

  glCreateVertexArrays(1, &vao);
  glCreateBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  glBindVertexArray(0);
  vertexData = new GLfloat[MAX_BUFFER_SIZE];

  fontShader.use();
  fontShader.setMat4("u_projection", projection);
  fontShader.setInt("u_fontTexture", TEX_FONT);
}

FontManager::~FontManager()
{
  delete[] vertexData;
}

void FontManager::resetBufferOffset()
{
  bufferOffset = 0;
  vertexCount = 0;
}

void FontManager::addText(std::string text, GLfloat x, GLfloat y, GLfloat scale)
{
  std::string::const_iterator c;
  GLfloat xpos = x;
  GLfloat ypos = y;
  for (c = text.begin(); c != text.end(); ++c)
    {
      Character ch = chars[*c];
      //vertex1 pos
      vertexData[bufferOffset++] = xpos + ch.xoffset * scale;
      vertexData[bufferOffset++] = ypos - ch.yoffset * scale;
      //vertex1 texCoords
      vertexData[bufferOffset++] = (float)ch.x / (float)textureWidth;
      vertexData[bufferOffset++] = (float)ch.y / (float)textureHeight;
      //vertex2 pos
      vertexData[bufferOffset++] = xpos + (ch.width + ch.xoffset) * scale;
      vertexData[bufferOffset++] = ypos - ch.yoffset * scale;
      //vertex2 texCoords
      vertexData[bufferOffset++] = (float)(ch.x + ch.width) / (float)textureWidth;
      vertexData[bufferOffset++] = (float)ch.y / (float)textureHeight;
      //vertex3 pos
      vertexData[bufferOffset++] = xpos + (ch.width + ch.xoffset) * scale;
      vertexData[bufferOffset++] = ypos - (ch.height + ch.yoffset) * scale;
      //vertex3 texCoords
      vertexData[bufferOffset++] = (float)(ch.x + ch.width) / (float)textureWidth;
      vertexData[bufferOffset++] = (float)(ch.y + ch.height) / (float)textureHeight;
      //vertex4 pos
      vertexData[bufferOffset++] = xpos + (ch.width + ch.xoffset) * scale;
      vertexData[bufferOffset++] = ypos - (ch.height + ch.yoffset) * scale;
      //vertex4 texCoords
      vertexData[bufferOffset++] = (float)(ch.x + ch.width) / (float)textureWidth;
      vertexData[bufferOffset++] = (float)(ch.y + ch.height) / (float)textureHeight;
      //vertex5 pos
      vertexData[bufferOffset++] = xpos + ch.xoffset * scale;
      vertexData[bufferOffset++] = ypos - (ch.height + ch.yoffset) * scale;
      //vertex5 texCoords
      vertexData[bufferOffset++] = (float)ch.x / (float)textureWidth;
      vertexData[bufferOffset++] = (float)(ch.y + ch.height) / (float)textureHeight;
      //vertex6 pos
      vertexData[bufferOffset++] = xpos + ch.xoffset * scale;
      vertexData[bufferOffset++] = ypos - ch.yoffset * scale;
      //vertex6 texCoords
      vertexData[bufferOffset++] = (float)ch.x / (float)textureWidth;
      vertexData[bufferOffset++] = (float)ch.y / (float)textureHeight;

      xpos += ch.xadvance * scale;
      ++vertexCount;
    }
}

void FontManager::drawText()
{
  fontShader.use();
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, bufferOffset * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);
  glDrawArrays(GL_TRIANGLES, 0, vertexCount * 6);
}
