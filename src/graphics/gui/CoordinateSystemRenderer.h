#ifndef COORDINATESYSTEMRENDERER_H
#define COORDINATESYSTEMRENDERER_H
#include "graphics/shaders/Shader.h"
#include "graphics/openglObjects/BufferCollection.h"

class CoordinateSystemRenderer
{
public:
  CoordinateSystemRenderer(Shader* shader);
  virtual ~CoordinateSystemRenderer() = default;
  void draw(const glm::mat3& view, float aspectRatio);

private:
  BufferCollection basicGLBuffers;
  Shader* shader;
};

#endif // COORDINATESYSTEMRENDERER_H
