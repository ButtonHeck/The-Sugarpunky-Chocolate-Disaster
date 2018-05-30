#include "FontManager.h"

FontManager::FontManager(const std::string &fontfile, glm::mat4 projection)
  :
    filename(fontfile),
    fontProjection(projection)
{

}

void FontManager::loadFont()
{
  FT_Library ft;
  if (FT_Init_FreeType(&ft))
    std::cerr << "FreeType error: could not init FreeType library\n";
  FT_Face face;
  if (FT_New_Face(ft, std::string(PROJ_PATH + "/fonts/" + filename).c_str(), 0, &face))
    std::cerr << "FreeType error: could not load font\n";
  FT_Set_Pixel_Sizes(face, 0, 48);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  for (GLubyte c = 0; c < 128; c++)
    {
      if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
          std::cerr << "FreeType error: could not load glyph " << char(c) << "(" << c << ")\n";
          continue;
        }
      GLuint glyphTexture;
      glGenTextures(1, &glyphTexture);
      glBindTexture(GL_TEXTURE_2D, glyphTexture);
      glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      Character character = {
        glyphTexture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        face->glyph->advance.x};
      characters.insert(std::pair<GLchar, Character>(c, character));
    }
  FT_Done_Face(face);
  FT_Done_FreeType(ft);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void FontManager::renderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
  glEnable(GL_BLEND);
  shader.use();
  shader.setMat4("u_projection", fontProjection);
  shader.setVec3("u_textColor", color.r, color.g, color.b);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(vao);
  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++)
    {
      Character ch = characters[*c];
      GLfloat xpos = x + ch.bearing.x * scale;
      GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;
      GLfloat w = ch.size.x * scale;
      GLfloat h = ch.size.y * scale;
      GLfloat vertices[6][4] = {
        {xpos,  ypos+h, 0.0, 0.0},
        {xpos,  ypos,   0.0, 1.0},
        {xpos+w,ypos,   1.0, 1.0},
        {xpos,  ypos+h, 0.0, 0.0},
        {xpos+w,ypos,   1.0, 1.0},
        {xpos+w,ypos+h, 1.0, 0.0}
      };
      glBindTexture(GL_TEXTURE_2D, ch.textureID);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      x += (ch.advance >> 6) * scale;
    }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint &FontManager::getVAO()
{
  return vao;
}

GLuint &FontManager::getVBO()
{
  return vbo;
}

void FontManager::deleteGLObjects()
{
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}
