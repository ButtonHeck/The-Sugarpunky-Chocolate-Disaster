#include "TextManager.h"

TextManager::TextManager(const std::string &fontFile, const std::string &fontTexture, Shader &shader)
  :
    fontLoader(fontFile, fontTexture),
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
  glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &ramSize);
  ramSizeFloatPercentage = (float)ramSize / 100;
}

TextManager::~TextManager()
{
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
}

void TextManager::addText(ScreenResolution& screenResolution,
                          Camera& camera, Options& options,
                          MouseInputManager& mouseInput,
                          const unsigned int fps,
                          const bool waterAnimatorIsWorking)
{
  using std::to_string;
  using std::string;
  float scrHeight = (float)screenResolution.getHeight();
  glm::vec3 viewPosition = camera.getPosition();
  addString("CPU UPS: " + to_string(fps), 10.0f, scrHeight - 15.0f, 0.18f);
  addString("Camera pos: " + to_string(viewPosition.x).substr(0,6) + ": "
                         + to_string(viewPosition.y).substr(0,6) + ": "
                         + to_string(viewPosition.z).substr(0,6), 10.0f, scrHeight - 35.0f, 0.18f);
  addString("Camera on map: " + to_string(camera.getMapCoordX()) + ": " + to_string(camera.getMapCoordZ()),
                         10.0f, scrHeight - 55.0f, 0.18f);
  addString("View dir: " + to_string(camera.getDirection().x).substr(0,6) + ": "
                         + to_string(camera.getDirection().y).substr(0,6) + ": "
                         + to_string(camera.getDirection().z).substr(0,6), 10.0f, scrHeight - 75.0f, 0.18f);
  const glm::vec3& cursorToViewportDirection = mouseInput.getCursorToViewportDirection();
  addString("Cursor at: " + (!options.get(OPT_SHOW_CURSOR) ? "inactive" : (to_string(cursorToViewportDirection.x).substr(0,6) + ": "
                         + to_string(cursorToViewportDirection.y).substr(0,6) + ": "
                         + to_string(cursorToViewportDirection.z).substr(0,6))), 10.0f, scrHeight - 95.0f, 0.18f);
  addString("Cursor on map: " + (!options.get(OPT_SHOW_CURSOR) ? "inactive" : (to_string(mouseInput.getCursorMapX()) + ": "
                         + to_string(mouseInput.getCursorMapZ()-1) + ", " + mouseInput.getCursorTileName())),
                         10.0f, scrHeight - 115.0f, 0.18f);
  addString("Water culling: " + (options.get(OPT_WATER_CULLING) ? string("On") : string("Off")), 10.0f, 20.0f, 0.18f);
  addString("Hills culling: " + (options.get(OPT_HILLS_CULLING) ? string("On") : string("Off")), 10.0f, 40.0f, 0.18f);
  addString("Water anim thread works: " + (waterAnimatorIsWorking ? string("On") : string("Off")), 10.0f, 60.0f, 0.18f);
  glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &ramAvailable);
  addString("RAM available: " + (to_string(ramAvailable)
                                               .append(", ")
                                               .append(to_string(ramAvailable / ramSizeFloatPercentage))
                                               .append("%")), 10.0f, 80.0f, 0.18f);
  addString("Models Phong: " + (options.get(OPT_MODELS_PHONG_SHADING) ? string("On") : string("Off")), 10.0f, 100.0f, 0.18f);
}

void TextManager::addString(std::string text, GLfloat x, GLfloat y, GLfloat scale)
{
  std::string::const_iterator c;
  GLfloat xpos = x;
  GLfloat ypos = y;
  const std::map<char, Character>& alphabet = fontLoader.getAlphabet();
  float textureWidth = (float)fontLoader.getTextureWidth();
  float textureHeight = (float)fontLoader.getTextureHeight();
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

void TextManager::drawText()
{
  shader.use();
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, bufferOffset * sizeof(GLfloat), vertexData.get(), GL_STATIC_DRAW);

  glEnable(GL_BLEND);
  glDisable(GL_CULL_FACE);
  glDrawArrays(GL_TRIANGLES, 0, vertexCount * VERTICES_PER_TILE);
  glDisable(GL_BLEND);
  glEnable(GL_CULL_FACE);

  bufferOffset = 0;
  vertexCount = 0;
}
