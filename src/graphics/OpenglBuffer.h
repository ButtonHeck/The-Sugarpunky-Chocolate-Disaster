#ifndef OPENGLBUFFER_H
#define OPENGLBUFFER_H
#include <GL/glew.h>
#include <unordered_map>

enum OPENGL_OBJECT : int
{
  VAO =           0x000001,
  VBO =           0x000010,
  INSTANCE_VBO =  0x000100,
  EBO =           0x001000,
  DIBO =          0x010000,
  TFBO =          0x100000
};

class OpenglBuffer
{
public:
  explicit OpenglBuffer(int flags);
  OpenglBuffer(OpenglBuffer&& old) noexcept;
  OpenglBuffer(const OpenglBuffer& copy);
  virtual ~OpenglBuffer();
  void create(int flags);
  void reserveNameForFutureStorage(int flags);
  void deleteBuffers();
  void deleteBuffer(int flag);
  GLuint& get(int flag);
  void bind(int flag);
  void add(int flag);
private:
  std::unordered_map<int, GLuint> objects;
};

#endif // OPENGLBUFFER_H
