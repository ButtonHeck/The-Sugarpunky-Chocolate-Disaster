#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H
#include <iomanip>
#include <glm/vec2.hpp>
#include "../../src/graphics/gui/FontLoader.h"
#include "../../src/graphics/shaders/Shader.h"
#include "../../src/graphics/openglObjects/BufferCollection.h"
#include "../../src/graphics/Camera.h"
#include "../../src/input/MouseInputManager.h"
#include "../../src/util/VRAM_Monitor.h"

class TextManager
{
public:
  TextManager(const std::string& fontFile, const std::string& fontTexture, Shader& shader, const ScreenResolution &screenResolution);
  virtual ~TextManager() = default;
  void addDebugText(const Camera &camera,
                    Options &options,
                    const MouseInputManager& mouseInput,
                    const glm::vec3 &sunPosition,
                    unsigned int fps);
  void drawText();

private:
  constexpr static unsigned int MAX_BUFFER_SIZE = 1024 * 24;
  const glm::vec2 DEFAULT_SCALE = glm::vec2(0.19f, 0.2f);

  struct GlyphVertex
  {
    constexpr static unsigned int NUMBER_OF_ELEMENTS = 4;
    GlyphVertex(glm::vec2 pos, glm::vec2 texCoords);
    float posX, posY;
    float texCoordX, texCoordY;
  };
  void addString(const std::string &text, GLfloat x, GLfloat y, const glm::vec2 &scale);

  FontLoader fontLoader;
  Shader& shader;
  const ScreenResolution& screenResolution;
  glm::vec2 lineOffset;
  glm::vec2 scale;
  BufferCollection basicGLBuffers;
  GLfloat vertexData[MAX_BUFFER_SIZE] = {0};
  int bufferOffset = 0, glyphsCount = 0;
};

#endif // TEXTMANAGER_H
