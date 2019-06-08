#ifndef COORDINATESYSTEMRENDERER_H
#define COORDINATESYSTEMRENDERER_H
#include "Shader"
#include "BufferCollection"

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
