#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H
#include <IL/il.h>
#include <GL/glew.h>
#include <string>
#include <vector>

class TextureLoader
{
public:
  TextureLoader();
  GLuint loadTexture(const std::string& path, GLenum wrapType);
  GLuint loadCubemap(std::vector<std::string>& faces);
};

#endif // TEXTURELOADER_H
