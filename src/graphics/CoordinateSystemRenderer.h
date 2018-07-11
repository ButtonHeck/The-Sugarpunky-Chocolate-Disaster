#ifndef COORDINATESYSTEMRENDERER_H
#define COORDINATESYSTEMRENDERER_H
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include "graphics/Shader.h"

class CoordinateSystemRenderer
{
public:
  CoordinateSystemRenderer(Shader* shader);
  ~CoordinateSystemRenderer();
  void deleteGLObjects();
  void draw(const glm::mat3& view, float aspect_ratio);
private:
  GLuint vao, vbo;
  Shader* shader;
};

#endif // COORDINATESYSTEMRENDERER_H
