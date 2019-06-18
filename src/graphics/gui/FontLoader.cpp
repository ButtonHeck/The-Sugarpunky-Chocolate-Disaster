/*
 * Copyright 2019 Ilya Malgin
 * FontLoader.cpp
 * This file is part of The Sugarpunky Chocolate Disaster project
 *
 * The Sugarpunky Chocolate Disaster project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The Sugarpunky Chocolate Disaster project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * See <http://www.gnu.org/licenses/>
 *
 * Purpose: contains definition for FontLoader class
 * @version 0.1.0
 */

#include "FontLoader"
#include "TextureUnits"
#include "Logger"

#include <IL/il.h>
#include <fstream>
#include <sstream>

/**
* @brief plain ctor. Loads font texture to OpenGL, parses font file to create full alphabet character by character
* @param fontFile text file representing each character parameters such as position in the texture, its width/height etc.
* @param textureFile image file containing symbols sheet
*/
FontLoader::FontLoader(const std::string &fontFile, const std::string &textureFile)
{
  //load font texture
  if (!ilLoadImage(textureFile.c_str()))
    Logger::log("Error loading font texture: %\n", textureFile.c_str());
  ILubyte* textureData = ilGetData();
  textureWidth = ilGetInteger(IL_IMAGE_WIDTH);
  textureHeight = ilGetInteger(IL_IMAGE_HEIGHT);
  glCreateTextures(GL_TEXTURE_2D, 1, &fontTexture);
  glActiveTexture(GL_TEXTURE0 + TEX_FONT);
  glBindTexture(GL_TEXTURE_2D, fontTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
  glGenerateTextureMipmap(fontTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  //read font file data
  std::ifstream inputStream(fontFile);
  std::stringstream stringBuffer;
  stringBuffer << inputStream.rdbuf();
  std::string token;
  stringBuffer >> token; //numberOfCharacters=
  stringBuffer >> token;
  unsigned int numberOfCharacters = std::stoi(token);

  //parse string buffer and create alphabet
  for (unsigned int characterIndex = 0; characterIndex < numberOfCharacters; ++characterIndex)
    {
      Character character;
      stringBuffer >> token; //char
      stringBuffer >> token; //id=
      token = token.substr(3);
      int code = std::stoi(token);
      character.asciiCode = code;
      stringBuffer >> token; //x=
      token = token.substr(2);
      int x = std::stoi(token);
      character.x = x;
      stringBuffer >> token; //y=
      token = token.substr(2);
      int y = std::stoi(token);
      character.y = y;
      stringBuffer >> token; //width=
      token = token.substr(6);
      int width = std::stoi(token);
      character.width = width;
      stringBuffer >> token; //height=
      token = token.substr(7);
      int height = std::stoi(token);
      character.height = height;
      stringBuffer >> token; //xoffset=
      token = token.substr(8);
      int xoffset = std::stoi(token);
      character.xoffset = xoffset;
      stringBuffer >> token; //yoffset=
      token = token.substr(8);
      int yoffset = std::stoi(token);
      character.yoffset = yoffset;
      stringBuffer >> token; //xadvance=
      token = token.substr(9);
      int xadvance = std::stoi(token);
      character.xadvance = xadvance;
      stringBuffer >> token; //page=0
      stringBuffer >> token; //chnl=0
      alphabet[code] = character;
    }
  inputStream.close();
}

std::unordered_map<char, Character> &FontLoader::getAlphabet() noexcept
{
  return alphabet;
}

GLuint FontLoader::getTextureWidth() const noexcept
{
  return textureWidth;
}

GLuint FontLoader::getTextureHeight() const noexcept
{
  return textureHeight;
}
