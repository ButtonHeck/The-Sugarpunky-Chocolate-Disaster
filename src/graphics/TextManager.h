#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H
#include <string>
#include <memory>
#include <GL/glew.h>
#include "graphics/FontLoader.h"
#include "graphics/Shader.h"
#include "graphics/ScreenResolution.h"
#include "game/Camera.h"
#include "game/Options.h"
#include "input/MouseInputManager.h"

class TextManager
{
public:
  TextManager(const std::string& fontFile, const std::string& fontTexture, Shader& shader);
  virtual ~TextManager();
  void addText(ScreenResolution& screenResolution,
               Camera& camera, Options& options,
               MouseInputManager& mouseInput,
               const unsigned int fps,
               const bool waterAnimatorIsWorking);
  void drawText();
private:
  void addString(std::string text, GLfloat x, GLfloat y, GLfloat scale);
  FontLoader fontLoader;
  Shader& shader;
  const int MAX_BUFFER_SIZE = 1024 * 24;
  std::unique_ptr<GLfloat[]> vertexData;
  int bufferOffset = 0, vertexCount = 0;
  GLuint vao, vbo;
  int ramAvailable, ramSize;
  float ramSizeFloatPercentage;
};

#endif // TEXTMANAGER_H
