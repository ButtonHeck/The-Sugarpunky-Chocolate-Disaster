#include "graphics/FontLoader.h"
#include <iostream>

FontLoader::FontLoader(const std::string &fontFile, const std::string &textureFile)
{
  if (!ilLoadImage(textureFile.c_str()))
    std::cerr << "Error loading font texture " << textureFile.c_str() << std::endl;
  ILubyte* data = ilGetData();
  textureWidth = ilGetInteger(IL_IMAGE_WIDTH);
  textureHeight = ilGetInteger(IL_IMAGE_HEIGHT);
  glCreateTextures(GL_TEXTURE_2D, 1, &fontTexture);
  glActiveTexture(GL_TEXTURE0 + TEX_FONT);
  glBindTexture(GL_TEXTURE_2D, fontTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  std::ifstream input(fontFile);
  std::stringstream stringBuf;
  stringBuf << input.rdbuf();

  for (unsigned int i = 0; i < 97; ++i)
    {
      Character c;
      std::string token;
      stringBuf >> token; //char
      stringBuf >> token; //id=
      token = token.substr(3);
      int code = std::stoi(token);
      c.asciiCode = code;
      stringBuf >> token; //x=
      token = token.substr(2);
      int x = std::stoi(token);
      c.x = x;
      stringBuf >> token; //y=
      token = token.substr(2);
      int y = std::stoi(token);
      c.y = y;
      stringBuf >> token; //width=
      token = token.substr(6);
      int width = std::stoi(token);
      c.width = width;
      stringBuf >> token; //height=
      token = token.substr(7);
      int height = std::stoi(token);
      c.height = height;
      stringBuf >> token; //xoffset=
      token = token.substr(8);
      int xoffset = std::stoi(token);
      c.xoffset = xoffset;
      stringBuf >> token; //yoffset=
      token = token.substr(8);
      int yoffset = std::stoi(token);
      c.yoffset = yoffset;
      stringBuf >> token; //xadvance=
      token = token.substr(9);
      int xadvance = std::stoi(token);
      c.xadvance = xadvance;
      stringBuf >> token; //page=0
      stringBuf >> token; //chnl=0
      alphabet[code] = c;
    }
  input.close();
}

std::unordered_map<char, Character> &FontLoader::getAlphabet()
{
  return alphabet;
}

GLuint FontLoader::getTextureWidth() const
{
  return textureWidth;
}

GLuint FontLoader::getTextureHeight() const
{
  return textureHeight;
}
