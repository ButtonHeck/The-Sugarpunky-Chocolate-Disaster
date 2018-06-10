#ifndef FONTMANAGER_H
#define FONTMANAGER_H
#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glew.h>
#include <map>
#include <string>
#include <glm/glm.hpp>
#include "src/game/Settings.h"
#include "src/graphics/Shader.h"

class FontManager
{
public:
  FontManager(const std::string& fontfile, glm::mat4 projection, Shader* shader);
  void renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(0.0f));
  GLuint& getVAO();
  GLuint& getVBO();
  void deleteGLObjects();
private:
  std::string filename;
  GLuint vbo, vao;
  struct Character {
    GLuint textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    FT_Pos advance;
  };
  std::map<GLchar, Character> characters;
  glm::mat4 fontProjection;
  Shader* shader;
};

#endif // FONTMANAGER_H
