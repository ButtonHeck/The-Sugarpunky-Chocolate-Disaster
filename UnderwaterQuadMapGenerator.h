#ifndef UNDERWATERQUADMAPGENERATOR_H
#define UNDERWATERQUADMAPGENERATOR_H
#include "MapGenerator.h"

class UnderwaterQuadMapGenerator : public MapGenerator
{
public:
  UnderwaterQuadMapGenerator();
  void fillQuadBufferData();
  void draw(GLuint& underwaterTexture);
  void deleteGLObjects();
  GLuint& getVAO();
  GLuint& getVBO();
  GLuint& getEBO();
private:
  GLuint vao, vbo, ebo;
};

#endif // UNDERWATERQUADMAPGENERATOR_H
