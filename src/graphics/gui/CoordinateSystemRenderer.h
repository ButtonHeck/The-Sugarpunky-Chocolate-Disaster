#pragma once

#include "BufferCollection"

#include <glm/mat3x3.hpp>

class Shader;

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
