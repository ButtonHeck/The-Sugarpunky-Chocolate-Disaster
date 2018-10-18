#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H
#include <string>
#include <memory>
#include "graphics/FontLoader.h"
#include "graphics/Shader.h"
#include "graphics/ScreenResolution.h"
#include "game/Camera.h"
#include "game/Options.h"
#include "input/MouseInputManager.h"
#include "util/VRAM_Monitor.h"
#include "util/BenchmarkTimer.h"

class TextManager
{
public:
  TextManager(const std::string& fontFile, const std::string& fontTexture, Shader& shader);
  virtual ~TextManager() = default;
  void addText(ScreenResolution& screenResolution,
               Camera& camera, Options& options,
               MouseInputManager& mouseInput,
               const unsigned int fps);
  void drawText();
private:
  struct GlyphVertex
  {
    GlyphVertex(glm::vec2 pos, glm::vec2 texCoords);
    float posX, posY;
    float texCoordX, texCoordY;
  };
  void addString(const std::string &text, GLfloat x, GLfloat y, GLfloat scale);
  FontLoader fontLoader;
  Shader& shader;
  OpenglBuffer basicGLBuffers;
  const int MAX_BUFFER_SIZE = 1024 * 24;
  std::unique_ptr<GLfloat[]> vertexData;
  int bufferOffset = 0, glyphsCount = 0;
};

#endif // TEXTMANAGER_H
