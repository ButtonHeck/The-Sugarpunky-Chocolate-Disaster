#ifndef FONTMANAGER_H
#define FONTMANAGER_H
#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glew.h>
#include <map>
#include <string>
#include <glm/glm.hpp>
#include "Settings.h"
#include "Shader.h"

class FontManager
{
public:
  FontManager(const std::string& fontfile);
  void loadFont();
  void renderText(Shader& shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(0.0f));
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
  glm::mat4 fontProjection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT);
};

#endif // FONTMANAGER_H
