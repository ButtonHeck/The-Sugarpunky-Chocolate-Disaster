/*
 * Copyright 2019 Ilya Malgin
 * ModelChunk.h
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
 * Purpose: contains declaration for ModelChunk class
 * @version 0.1.0
 */

#ifndef MODELCHUNK_H
#define MODELCHUNK_H
#include <vector>
#include "game/world/Chunk.h"

/**
 * @brief Represents chunk storage for all models. Responsible for keeping information about
 * each model instances and their offsets kept in a particular chunk
 */
class ModelChunk : public Chunk
{
public:
  ModelChunk(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom, float height);

  //instance offsets (one unsigned int per model)
  void setInstanceOffsetsVector(std::vector<unsigned int>& instanceOffsets);
  void setInstanceOffset(unsigned int index, unsigned int offset);
  unsigned int getInstanceOffset(int index) const;
  std::vector<unsigned int>& getInstanceOffsetVector();

  //number of models instances (one unsigned int per model)
  void setNumInstancesVector(std::vector<unsigned int>& numInstances);
  void setNumInstances(unsigned int index, unsigned int instances);
  unsigned int getNumInstances(int index) const;
  std::vector<unsigned int>& getNumInstancesVector();

private:
  std::vector<unsigned int> instanceOffsets;
  std::vector<unsigned int> numInstances;
};

inline void ModelChunk::setInstanceOffsetsVector(std::vector<unsigned int> &instanceOffsets)
{
  for (unsigned int i = 0; i < instanceOffsets.size(); i++)
    this->instanceOffsets.emplace_back(instanceOffsets[i]);
}

inline void ModelChunk::setInstanceOffset(unsigned int index, unsigned int offset)
{
  instanceOffsets[index] = offset;
}

inline void ModelChunk::setNumInstancesVector(std::vector<unsigned int> &numInstances)
{
  for (unsigned int i = 0; i < numInstances.size(); i++)
    this->numInstances.emplace_back(numInstances[i]);
}

inline void ModelChunk::setNumInstances(unsigned int index, unsigned int instances)
{
  numInstances[index] = instances;
}

inline unsigned int ModelChunk::getInstanceOffset(int index) const
{
  return instanceOffsets[index];
}

inline std::vector<unsigned int> &ModelChunk::getInstanceOffsetVector()
{
  return instanceOffsets;
}

inline unsigned int ModelChunk::getNumInstances(int index) const
{
  return numInstances[index];
}

inline std::vector<unsigned int> &ModelChunk::getNumInstancesVector()
{
  return numInstances;
}

#endif // MODELCHUNK_H
