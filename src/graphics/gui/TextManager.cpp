#include "graphics/gui/TextManager.h"

TextManager::TextManager(const std::string &fontFile, const std::string &fontTexture, Shader &shader, const ScreenResolution &screenResolution)
  :
    fontLoader(fontFile, fontTexture),
    shader(shader),
    screenResolution(screenResolution),
    lineOffset(screenResolution.getWidthRatioToReference(), screenResolution.getHeightRatioToReference()),
    scale(DEFAULT_SCALE.x * lineOffset.x, DEFAULT_SCALE.y * lineOffset.y),
    basicGLBuffers(VAO | VBO)
{
  basicGLBuffers.bind(VAO | VBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, GlyphVertex::NUMBER_OF_ELEMENTS, GL_FLOAT, GL_FALSE, GlyphVertex::NUMBER_OF_ELEMENTS * sizeof(GLfloat), 0);
  BufferCollection::bindZero(VAO | VBO);
}

void TextManager::addDebugText(const Camera& camera,
                               Options &options,
                               const MouseInputManager &mouseInput,
                               const glm::vec3& sunPosition,
                               unsigned int fps)
{
  BENCHMARK("TextManager: add text", true);
  float screenHeight = (float)screenResolution.getHeight();
  glm::vec3 viewPosition = camera.getPosition();
  std::stringstream ss;
  const float CROSSLINE_OFFSET_Y = 21.0f;
  const float LEFT_BORDER_OFFSET = 10.0f;
  const float UPPER_BORDER_OFFSET = 15.0f;
  const float LOWER_BORDER_OFFSET = 20.0f;
  unsigned int lineCounter = 0;

  ss << "FPS: " << fps << std::setprecision(5) << std::setw(5)
     << ", Frame time: " << 1.0f / fps << "ms";
  addString(ss.str(), LEFT_BORDER_OFFSET * lineOffset.x, screenHeight - (UPPER_BORDER_OFFSET + CROSSLINE_OFFSET_Y * lineCounter++) * lineOffset.y, scale);

  ss.str("");
  ss << "Camera pos: " << std::setprecision(4) << std::setw(4)
     << viewPosition.x << ": " << viewPosition.y << ": " << viewPosition.z;
  addString(ss.str(), LEFT_BORDER_OFFSET * lineOffset.x, screenHeight - (UPPER_BORDER_OFFSET + CROSSLINE_OFFSET_Y * lineCounter++) * lineOffset.y, scale);

  ss.str("");
  glm::vec2 cameraWorldCoords = camera.getWorldCoordinates();
  ss << "Camera on map: " << cameraWorldCoords.x << ": " << cameraWorldCoords.y;
  addString(ss.str(), LEFT_BORDER_OFFSET * lineOffset.x, screenHeight - (UPPER_BORDER_OFFSET + CROSSLINE_OFFSET_Y * lineCounter++) * lineOffset.y, scale);

  ss.str("");
  ss << "View dir: " << std::setprecision(3) << std::setw(3)
     << camera.getDirection().x << ": " << camera.getDirection().y << ": " << camera.getDirection().z;
  addString(ss.str(), LEFT_BORDER_OFFSET * lineOffset.x, screenHeight - (UPPER_BORDER_OFFSET + CROSSLINE_OFFSET_Y * lineCounter++) * lineOffset.y, scale);

  ss.str("");
  ss << "Cursor on map: ";
  if (!options[OPT_SHOW_CURSOR])
    ss << "inactive";
  else
    {
      ss << std::setprecision(3) << std::setw(3)
         << mouseInput.getCursorWorldX() << ": " << mouseInput.getCursorWorldZ()-1 << ", " << mouseInput.getCursorTileName();
    }
  addString(ss.str(), LEFT_BORDER_OFFSET * lineOffset.x, screenHeight - (UPPER_BORDER_OFFSET + CROSSLINE_OFFSET_Y * lineCounter++) * lineOffset.y, scale);

  lineCounter = 0;
  ss.str("");
  ss << "Water culling: " << std::boolalpha << options[OPT_WATER_CULLING];
  addString(ss.str(), LEFT_BORDER_OFFSET * lineOffset.x, (LOWER_BORDER_OFFSET + CROSSLINE_OFFSET_Y * lineCounter++) * lineOffset.y, scale);

  ss.str("");
  ss << "Hills culling: " << std::boolalpha << options[OPT_HILLS_CULLING];
  addString(ss.str(), LEFT_BORDER_OFFSET * lineOffset.x, (LOWER_BORDER_OFFSET + CROSSLINE_OFFSET_Y * lineCounter++) * lineOffset.y, scale);

  ss.str("");
  ss << "Models Phong: " << std::boolalpha << options[OPT_MODELS_PHONG_SHADING];
  addString(ss.str(), LEFT_BORDER_OFFSET * lineOffset.x, (LOWER_BORDER_OFFSET + CROSSLINE_OFFSET_Y * lineCounter++) * lineOffset.y, scale);

  ss.str("");
  ss << "Shadow cam fixed: " << std::boolalpha << options[OPT_SHADOW_CAMERA_FIXED];
  addString(ss.str(), LEFT_BORDER_OFFSET * lineOffset.x, (LOWER_BORDER_OFFSET + CROSSLINE_OFFSET_Y * lineCounter++) * lineOffset.y, scale);

  VRAM_Monitor& vram = VRAM_Monitor::getInstance();
  vram.updateAvailable();
  ss.str("");
  ss << "RAM available: " << std::setprecision(6) << std::setw(8)
     << vram.getAvailableMemory() << ", " << vram.getAvailableMemoryPercent() << "%";
  addString(ss.str(), LEFT_BORDER_OFFSET * lineOffset.x, (LOWER_BORDER_OFFSET + CROSSLINE_OFFSET_Y * lineCounter++) * lineOffset.y, scale);

  ss.str("");
  ss << "Sun position: " << std::setprecision(3) << std::setw(3)
     << sunPosition.x << ": " << sunPosition.y << ": " << sunPosition.z;
  addString(ss.str(), LEFT_BORDER_OFFSET * lineOffset.x, (LOWER_BORDER_OFFSET + CROSSLINE_OFFSET_Y * lineCounter++) * lineOffset.y, scale);
}

void TextManager::addString(const std::string& text, GLfloat x, GLfloat y, const glm::vec2 &scale)
{
  std::string::const_iterator characterIterator;
  std::unordered_map<char, Character>& alphabet = fontLoader.getAlphabet();
  float textureWidth = (float)fontLoader.getTextureWidth();
  float textureHeight = (float)fontLoader.getTextureHeight();
  for (characterIterator = text.begin(); characterIterator != text.end(); ++characterIterator)
    {
      Character& character = alphabet[*characterIterator];
      GlyphVertex lowLeft(glm::vec2(x + character.xoffset * scale.x, y - character.yoffset * scale.y),
                          glm::vec2((float)character.x / textureWidth, (float)character.y / textureHeight));
      GlyphVertex lowRight(glm::vec2(x + (character.width + character.xoffset) * scale.x, y - character.yoffset * scale.y),
                           glm::vec2((float)(character.x + character.width) / textureWidth, (float)character.y / textureHeight));
      GlyphVertex upRight(glm::vec2(x + (character.width + character.xoffset) * scale.x, y - (character.height + character.yoffset) * scale.y),
                          glm::vec2((float)(character.x + character.width) / textureWidth, (float)(character.y + character.height) / textureHeight));
      GlyphVertex upLeft(glm::vec2(x + character.xoffset * scale.x, y - (character.height + character.yoffset) * scale.y),
                         glm::vec2((float)character.x / textureWidth, (float)(character.y + character.height) / textureHeight));

      vertexData[bufferOffset++] = lowLeft.posX;
      vertexData[bufferOffset++] = lowLeft.posY;
      vertexData[bufferOffset++] = lowLeft.texCoordX;
      vertexData[bufferOffset++] = lowLeft.texCoordY;

      vertexData[bufferOffset++] = upLeft.posX;
      vertexData[bufferOffset++] = upLeft.posY;
      vertexData[bufferOffset++] = upLeft.texCoordX;
      vertexData[bufferOffset++] = upLeft.texCoordY;

      vertexData[bufferOffset++] = upRight.posX;
      vertexData[bufferOffset++] = upRight.posY;
      vertexData[bufferOffset++] = upRight.texCoordX;
      vertexData[bufferOffset++] = upRight.texCoordY;

      vertexData[bufferOffset++] = upRight.posX;
      vertexData[bufferOffset++] = upRight.posY;
      vertexData[bufferOffset++] = upRight.texCoordX;
      vertexData[bufferOffset++] = upRight.texCoordY;

      vertexData[bufferOffset++] = lowRight.posX;
      vertexData[bufferOffset++] = lowRight.posY;
      vertexData[bufferOffset++] = lowRight.texCoordX;
      vertexData[bufferOffset++] = lowRight.texCoordY;

      vertexData[bufferOffset++] = lowLeft.posX;
      vertexData[bufferOffset++] = lowLeft.posY;
      vertexData[bufferOffset++] = lowLeft.texCoordX;
      vertexData[bufferOffset++] = lowLeft.texCoordY;

      x += character.xadvance * scale.x;
    }
  glyphsCount += text.size();
}

void TextManager::drawText()
{
  BENCHMARK("TextManager: draw", true);
  shader.use();
  basicGLBuffers.bind(VAO | VBO);
  glBufferData(GL_ARRAY_BUFFER, bufferOffset * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);
  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glDrawArrays(GL_TRIANGLES, 0, glyphsCount * VERTICES_PER_QUAD);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);

  bufferOffset = 0;
  glyphsCount = 0;
}

TextManager::GlyphVertex::GlyphVertex(glm::vec2 pos, glm::vec2 texCoords)
  :
    posX(pos.x), posY(pos.y),
    texCoordX(texCoords.x), texCoordY(texCoords.y)
{}
