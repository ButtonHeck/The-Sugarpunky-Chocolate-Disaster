#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H
#include <IL/il.h>
#include <GL/glew.h>
#include <string>

class TextureLoader
{
public:
  TextureLoader();
  GLuint loadTexture(const std::string& path, GLenum wrapType);
};

#endif // TEXTURELOADER_H
