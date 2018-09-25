#ifndef DEPTHMAPBUFFER_H
#define DEPTHMAPBUFFER_H
#include <GL/glew.h>
#include <iostream>

class DepthmapBuffer
{
public:
  DepthmapBuffer() = default;
  ~DepthmapBuffer();
  void setupBuffer(GLuint depthTextureUnit);
  void prepareBuffer(int frameWidth, int frameHeight);
  void restoreDefaultBuffer(int frameWidth, int frameHeight);
private:
  GLuint FBO;
};

#endif // DEPTHMAPBUFFER_H
