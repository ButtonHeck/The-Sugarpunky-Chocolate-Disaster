#ifndef UNDERWATERQUADMAPGENERATOR_H
#define UNDERWATERQUADMAPGENERATOR_H
#include "MapGenerator.h"

class UnderwaterQuadMapGenerator : public MapGenerator
{
public:
  UnderwaterQuadMapGenerator();
  void fillQuadBufferData();
  void draw(GLuint& underwaterTexture);
};

#endif // UNDERWATERQUADMAPGENERATOR_H
