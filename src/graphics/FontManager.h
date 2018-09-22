#ifndef FONTMANAGER_H
#define FONTMANAGER_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <IL/il.h>
#include <map>
#include "game/TextureUnits.h"
#include "graphics/Shader.h"

class FontManager
{
public:
  FontManager(const std::string& fontFile, const std::string& fontTexture, glm::mat4 projection, Shader& shader);
  ~FontManager();
  void resetBufferOffset();
  void addText(std::string text, GLfloat x, GLfloat y, GLfloat scale);
  void drawText();
private:
  struct Character {
    int asciiCode, x, y, width, height, xoffset, yoffset, xadvance;
  };
  Shader& fontShader;
  const int MAX_BUFFER_SIZE = 1024 * 24;
  GLfloat* vertexData;
  int bufferOffset = 0, vertexCount = 0;
  std::map<char, Character> chars;
  GLuint fontTexture, textureWidth, textureHeight;
  GLuint vao, vbo;
};

#endif // FONTMANAGER_H
