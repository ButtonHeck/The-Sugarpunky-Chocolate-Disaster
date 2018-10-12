#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H
#include <string>
#include <memory>
#include <GL/glew.h>
#include "graphics/FontLoader.h"
#include "graphics/Shader.h"

class TextRenderer
{
public:
  TextRenderer(FontLoader& font, Shader& shader);
  virtual ~TextRenderer() = default;
  void addText(std::string text, GLfloat x, GLfloat y, GLfloat scale);
  void drawText();
private:
  FontLoader& font;
  Shader& shader;
  const int MAX_BUFFER_SIZE = 1024 * 24;
  std::unique_ptr<GLfloat[]> vertexData;
  int bufferOffset = 0, vertexCount = 0;
  GLuint vao, vbo;
};

#endif // TEXTRENDERER_H
