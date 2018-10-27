#include "graphics/TextManager.h"

TextManager::TextManager(const std::string &fontFile, const std::string &fontTexture, Shader &shader)
  :
    fontLoader(fontFile, fontTexture),
    shader(shader),
    basicGLBuffers(VAO | VBO),
    vertexData(new GLfloat[MAX_BUFFER_SIZE])
{
  basicGLBuffers.bind(VAO | VBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  glBindVertexArray(0);
}

void TextManager::addText(ScreenResolution& screenResolution,
                          Camera& camera, Options& options,
                          MouseInputManager& mouseInput,
                          const unsigned int fps)
{
  BENCHMARK("Text: add text", true);
  float scrHeight = (float)screenResolution.getHeight();
  glm::vec3 viewPosition = camera.getPosition();
  std::stringstream ss;

  ss << "CPU UPS: " << fps;
  addString(ss.str(), 10.0f, scrHeight - 15.0f, 0.18f);

  ss.str("");
  ss << "Camera pos: " << std::setprecision(4) << std::setw(4)
     << viewPosition.x
     << ": "
     << viewPosition.y
     << ": "
     << viewPosition.z;
  addString(ss.str(), 10.0f, scrHeight - 35.0f, 0.18f);

  ss.str("");
  ss << "Camera on map: "
     << camera.getMapCoordX()
     << ": "
     << camera.getMapCoordZ();
  addString(ss.str(), 10.0f, scrHeight - 55.0f, 0.18f);

  ss.str("");
  ss << "View dir: " << std::setprecision(3) << std::setw(3)
     << camera.getDirection().x
     << ": "
     << camera.getDirection().y
     << ": "
     << camera.getDirection().z;
  addString(ss.str(), 10.0f, scrHeight - 75.0f, 0.18f);

  const glm::vec3& cursorToViewportDirection = mouseInput.getCursorToViewportDirection();
  ss.str("");
  ss << "Cursor at: ";
  if (!options.get(OPT_SHOW_CURSOR))
    ss << "inactive";
  else
    {
      ss << std::setprecision(3) << std::setw(3)
         << cursorToViewportDirection.x
         << ": "
         << cursorToViewportDirection.y
         << ": "
         << cursorToViewportDirection.z;
    }
  addString(ss.str(), 10.0f, scrHeight - 95.0f, 0.18f);

  ss.str("");
  ss << "Cursor on map: ";
  if (!options.get(OPT_SHOW_CURSOR))
    ss << "inactive";
  else
    {
      ss << std::setprecision(3) << std::setw(3)
         << mouseInput.getCursorMapX()
         << ": "
         << mouseInput.getCursorMapZ()-1
         << ", "
         << mouseInput.getCursorTileName();
    }
  addString(ss.str(), 10.0f, scrHeight - 115.0f, 0.18f);

  ss.str("");
  ss << "Water culling: " << std::boolalpha << options.get(OPT_WATER_CULLING);
  addString(ss.str(), 10.0f, 20.0f, 0.18f);

  ss.str("");
  ss << "Hills culling: " << std::boolalpha << options.get(OPT_HILLS_CULLING);
  addString(ss.str(), 10.0f, 40.0f, 0.18f);

  ss.str("");
  ss << "Models Phong: " << std::boolalpha << options.get(OPT_MODELS_PHONG_SHADING);
  addString(ss.str(), 10.0f, 60.0f, 0.18f);

  VRAM_Monitor& vram = VRAM_Monitor::getInstance();
  vram.updateAvailable();
  ss.str("");
  ss << "RAM available: " << std::setprecision(6) << std::setw(8)
     << vram.getAvailableMemory()
     << ", "
     << vram.getAvailableMemoryPercent()
     << "%";
  addString(ss.str(), 10.0f, 80.0f, 0.18f);
}

void TextManager::addString(const std::string& text, GLfloat x, GLfloat y, GLfloat scale)
{
  std::string::const_iterator c;
  std::unordered_map<char, Character>& alphabet = fontLoader.getAlphabet();
  float textureWidth = (float)fontLoader.getTextureWidth();
  float textureHeight = (float)fontLoader.getTextureHeight();
  for (c = text.begin(); c != text.end(); ++c)
    {
      Character& ch = alphabet[*c];
      GlyphVertex lowLeft(glm::vec2(x + ch.xoffset * scale, y - ch.yoffset * scale),
                          glm::vec2((float)ch.x / textureWidth, (float)ch.y / textureHeight));
      GlyphVertex lowRight(glm::vec2(x + (ch.width + ch.xoffset) * scale, y - ch.yoffset * scale),
                           glm::vec2((float)(ch.x + ch.width) / textureWidth, (float)ch.y / textureHeight));
      GlyphVertex upRight(glm::vec2(x + (ch.width + ch.xoffset) * scale, y - (ch.height + ch.yoffset) * scale),
                          glm::vec2((float)(ch.x + ch.width) / textureWidth, (float)(ch.y + ch.height) / textureHeight));
      GlyphVertex upLeft(glm::vec2(x + ch.xoffset * scale, y - (ch.height + ch.yoffset) * scale),
                         glm::vec2((float)ch.x / textureWidth, (float)(ch.y + ch.height) / textureHeight));

      vertexData[bufferOffset++] = lowLeft.posX;
      vertexData[bufferOffset++] = lowLeft.posY;
      vertexData[bufferOffset++] = lowLeft.texCoordX;
      vertexData[bufferOffset++] = lowLeft.texCoordY;

      vertexData[bufferOffset++] = lowRight.posX;
      vertexData[bufferOffset++] = lowRight.posY;
      vertexData[bufferOffset++] = lowRight.texCoordX;
      vertexData[bufferOffset++] = lowRight.texCoordY;

      vertexData[bufferOffset++] = upRight.posX;
      vertexData[bufferOffset++] = upRight.posY;
      vertexData[bufferOffset++] = upRight.texCoordX;
      vertexData[bufferOffset++] = upRight.texCoordY;

      vertexData[bufferOffset++] = upRight.posX;
      vertexData[bufferOffset++] = upRight.posY;
      vertexData[bufferOffset++] = upRight.texCoordX;
      vertexData[bufferOffset++] = upRight.texCoordY;

      vertexData[bufferOffset++] = upLeft.posX;
      vertexData[bufferOffset++] = upLeft.posY;
      vertexData[bufferOffset++] = upLeft.texCoordX;
      vertexData[bufferOffset++] = upLeft.texCoordY;

      vertexData[bufferOffset++] = lowLeft.posX;
      vertexData[bufferOffset++] = lowLeft.posY;
      vertexData[bufferOffset++] = lowLeft.texCoordX;
      vertexData[bufferOffset++] = lowLeft.texCoordY;

      x += ch.xadvance * scale;
    }
  glyphsCount += text.size();
}

void TextManager::drawText()
{
  shader.use();
  basicGLBuffers.bind(VAO | VBO);
  glBufferData(GL_ARRAY_BUFFER, bufferOffset * sizeof(GLfloat), vertexData.get(), GL_STATIC_DRAW);

  glEnable(GL_BLEND);
  glDisable(GL_CULL_FACE);
  glDrawArrays(GL_TRIANGLES, 0, glyphsCount * VERTICES_PER_TILE);
  glDisable(GL_BLEND);
  glEnable(GL_CULL_FACE);

  bufferOffset = 0;
  glyphsCount = 0;
}

TextManager::GlyphVertex::GlyphVertex(glm::vec2 pos, glm::vec2 texCoords)
  :
    posX(pos.x), posY(pos.y),
    texCoordX(texCoords.x), texCoordY(texCoords.y)
{}
