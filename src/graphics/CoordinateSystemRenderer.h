#ifndef COORDINATESYSTEMRENDERER_H
#define COORDINATESYSTEMRENDERER_H
#include "graphics/Shader.h"
#include "graphics/OpenglBuffer.h"

class CoordinateSystemRenderer
{
public:
  CoordinateSystemRenderer(Shader* shader);
  virtual ~CoordinateSystemRenderer() = default;
  void draw(const glm::mat3& view, float aspect_ratio);
private:
  OpenglBuffer basicGLBuffers;
  Shader* shader;
};

#endif // COORDINATESYSTEMRENDERER_H
