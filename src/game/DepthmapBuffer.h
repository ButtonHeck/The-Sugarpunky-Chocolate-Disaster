#ifndef DEPTHMAPBUFFER_H
#define DEPTHMAPBUFFER_H
#include <GL/glew.h>
#include <iostream>

class DepthmapBuffer
{
public:
  DepthmapBuffer() = default;
  virtual ~DepthmapBuffer();
  void setup(GLuint depthTextureUnit);
  void bindToViewport(int frameWidth, int frameHeight);
  void unbindToViewport(int frameWidth, int frameHeight);
private:
  GLuint FBO;
};

#endif // DEPTHMAPBUFFER_H
