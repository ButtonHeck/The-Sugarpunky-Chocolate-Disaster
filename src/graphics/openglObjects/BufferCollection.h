#pragma once

#include <GL/glew.h>
#include <unordered_map>

constexpr unsigned char QUAD_INDICES[6] = {0,1,2,2,3,0};
constexpr unsigned int VERTICES_PER_QUAD = 6;
constexpr unsigned int INDIRECT_DRAW_COMMAND_ARGUMENTS = 5;
constexpr GLsizeiptr INDIRECT_DRAW_COMMAND_BYTE_SIZE = sizeof(GLuint) * INDIRECT_DRAW_COMMAND_ARGUMENTS;

enum OPENGL_OBJECT : int
{
  VAO =           0x000001,
  VBO =           0x000010,
  INSTANCE_VBO =  0x000100,
  EBO =           0x001000,
  DIBO =          0x010000,
  TFBO =          0x100000
};

class BufferCollection
{
public:
  BufferCollection() = default;
  explicit BufferCollection(int flags);
  BufferCollection(BufferCollection&& old) noexcept;
  BufferCollection(BufferCollection& copy);
  virtual ~BufferCollection();
  static void bindZero(int flags) noexcept;
  void create(int flags);
  void deleteBuffers();
  void deleteBuffer(int flag);
  GLuint& get(int flag);
  void bind(int flag);
  void add(int flag);

private:
  std::unordered_map<int, GLuint> objects;
};
