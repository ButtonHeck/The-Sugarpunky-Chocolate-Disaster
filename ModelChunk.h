#ifndef MODELCHUNK_H
#define MODELCHUNK_H
#include "Settings.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class ModelChunk
{
public:
  ModelChunk(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom);
  void setInstanceOffsets(std::vector<unsigned int>& instanceOffsets);
  void setNumInstances(std::vector<unsigned int>& numInstances);
  unsigned int getInstanceOffset(int index);
  unsigned int getNumInstances(int index);
  bool containsPoint(const glm::vec2& position);
  unsigned int getLeft();
  unsigned int getRight();
  unsigned int getTop();
  unsigned int getBottom();
private:
  unsigned int left, right, top, bottom;
  std::vector<unsigned int> instanceOffsets;
  std::vector<unsigned int> numInstances;
};

#endif // MODELCHUNK_H
