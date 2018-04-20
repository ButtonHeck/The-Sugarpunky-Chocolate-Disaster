#ifndef COORDINATESYSTEMRENDERER_H
#define COORDINATESYSTEMRENDERER_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Settings.h"
#include "Shader.h"

class CoordinateSystemRenderer
{
public:
  CoordinateSystemRenderer() = default;
  void fillBufferData();
  void deleteGLObjects();
  void draw(Shader& shader, glm::mat4& view, float aspect_ratio);
private:
  GLuint vao, vbo;
};

#endif // COORDINATESYSTEMRENDERER_H
