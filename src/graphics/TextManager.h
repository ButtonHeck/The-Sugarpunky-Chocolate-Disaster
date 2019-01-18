#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H
#include <string>
#include <memory>
#include <iomanip>
#include <glm/vec2.hpp>
#include "graphics/FontLoader.h"
#include "graphics/Shader.h"
#include "graphics/ScreenResolution.h"
#include "graphics/OpenglBuffer.h"
#include "game/Camera.h"
#include "game/Options.h"
#include "input/MouseInputManager.h"
#include "util/VRAM_Monitor.h"
#include "util/BenchmarkTimer.h"

class TextManager
{
public:
  TextManager(const std::string& fontFile, const std::string& fontTexture, Shader& shader, ScreenResolution& screenResolution);
  virtual ~TextManager() = default;
  void addText(Camera& camera, Options& options,
               MouseInputManager& mouseInput, glm::vec3 sunPosition,
               const unsigned int fps);
  void drawText();
private:
  const int MAX_BUFFER_SIZE = 1024 * 24;
  const glm::vec2 DEFAULT_SCALE = glm::vec2(0.2f, 0.2f);
  struct GlyphVertex
  {
    GlyphVertex(glm::vec2 pos, glm::vec2 texCoords);
    float posX, posY;
    float texCoordX, texCoordY;
  };
  void addString(const std::string &text, GLfloat x, GLfloat y, glm::vec2 &scale);
  FontLoader fontLoader;
  Shader& shader;
  ScreenResolution& screenResolution;
  glm::vec2 lineOffset;
  glm::vec2 scale;
  OpenglBuffer basicGLBuffers;
  std::unique_ptr<GLfloat[]> vertexData;
  int bufferOffset = 0, glyphsCount = 0;
};

#endif // TEXTMANAGER_H
