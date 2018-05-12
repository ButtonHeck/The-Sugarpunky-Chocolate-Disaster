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
  void setInstanceOffsetsVector(std::vector<unsigned int>& instanceOffsets);
  void setInstanceOffset(unsigned int index, unsigned int offset);
  void setNumInstancesVector(std::vector<unsigned int>& numInstances);
  void setNumInstances(unsigned int index, unsigned int instances);
  unsigned int getInstanceOffset(int index);
  std::vector<unsigned int>& getInstanceOffsetVector();
  unsigned int getNumInstances(int index);
  std::vector<unsigned int>& getNumInstancesVector();
  bool containsPoint(float x, float y);
  unsigned int getLeft();
  unsigned int getRight();
  unsigned int getTop();
  unsigned int getBottom();
  glm::vec2 getMidPoint();
private:
  unsigned int left, right, top, bottom;
  float midPointX, midPointY;
  std::vector<unsigned int> instanceOffsets;
  std::vector<unsigned int> numInstances;
};

#endif // MODELCHUNK_H
