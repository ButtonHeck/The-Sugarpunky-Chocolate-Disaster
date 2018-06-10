#ifndef COORDINATESYSTEMRENDERER_H
#define COORDINATESYSTEMRENDERER_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "src/game/Settings.h"
#include "src/graphics/Shader.h"

class CoordinateSystemRenderer
{
public:
  CoordinateSystemRenderer(Shader* shader);
  void deleteGLObjects();
  void draw(const glm::mat4& view, float aspect_ratio);
private:
  GLuint vao, vbo;
  Shader* shader;
};

#endif // COORDINATESYSTEMRENDERER_H
