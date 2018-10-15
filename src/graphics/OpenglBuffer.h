#ifndef OPENGLBUFFER_H
#define OPENGLBUFFER_H
#include <GL/glew.h>
#include <unordered_map>

enum OPENGL_OBJECT : int
{
  VAO =           0x00001,
  VBO =           0x00010,
  INSTANCE_VBO =  0x00100,
  EBO =           0x01000,
  DIBO =          0x10000
};

class OpenglBuffer
{
public:
  explicit OpenglBuffer(int flags);
  virtual ~OpenglBuffer();
  GLuint& get(int flag);
  void bind(int flag);
  void add(int flag);
private:
  std::unordered_map<int, GLuint> objects;
};

#endif // OPENGLBUFFER_H
