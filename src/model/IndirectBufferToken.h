#ifndef INDIRECTBUFFERTOKEN_H
#define INDIRECTBUFFERTOKEN_H
#include <GL/glew.h>

class IndirectBufferToken
{
public:
  IndirectBufferToken(GLuint indicesCount, GLuint numInstances, GLuint instanceOffset);
private:
  //{ indicesCount, numInstancesToDraw, firstIndex, baseVertex, baseInstance }
  GLuint indicesCount, numInstances, instanceOffset;
  static const GLuint FIRST_INDEX = 0, BASE_VERTEX = 0;
  friend class Mesh;
};

#endif // INDIRECTBUFFERTOKEN_H
