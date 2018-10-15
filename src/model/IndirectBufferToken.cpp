#include "model/IndirectBufferToken.h"

IndirectBufferToken::IndirectBufferToken(GLuint indicesCount, GLuint numInstances, GLuint instanceOffset)
  :
    indicesCount(indicesCount),
    numInstances(numInstances),
    instanceOffset(instanceOffset)
{}
