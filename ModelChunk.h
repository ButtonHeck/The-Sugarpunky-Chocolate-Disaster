#ifndef MODELCHUNK_H
#define MODELCHUNK_H
#include "Settings.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ModelChunk
{
public:
  ModelChunk(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom);
  void setInstanceOffset(unsigned int instanceOffset);
  void setNumInstances(unsigned int numInstances);
  unsigned int getInstanceOffset();
  unsigned int getNumInstances();
  bool containsPoint(const glm::vec2& position);
  unsigned int getLeft();
  unsigned int getRight();
  unsigned int getTop();
  unsigned int getBottom();
private:
  unsigned int left, right, top, bottom;
  unsigned int instanceOffset;
  unsigned int numInstances;
};

#endif // MODELCHUNK_H
