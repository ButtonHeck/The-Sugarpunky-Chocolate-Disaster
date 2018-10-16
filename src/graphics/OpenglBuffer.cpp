#include "OpenglBuffer.h"

OpenglBuffer::OpenglBuffer(int flags)
{
  create(flags);
}

OpenglBuffer::~OpenglBuffer()
{
  deleteBuffers();
}

void OpenglBuffer::create(int flags)
{
  if (flags & VAO)
    {
      GLuint vao;
      glCreateVertexArrays(1, &vao);
      objects[VAO] = vao;
    }
  if (flags & VBO)
    {
      GLuint vbo;
      glCreateBuffers(1, &vbo);
      objects[VBO] = vbo;
    }
  if (flags & INSTANCE_VBO)
    {
      GLuint vbo;
      glCreateBuffers(1, &vbo);
      objects[INSTANCE_VBO] = vbo;
    }
  if (flags & EBO)
    {
      GLuint ebo;
      glCreateBuffers(1, &ebo);
      objects[EBO] = ebo;
    }
  if (flags & DIBO)
    {
      GLuint dibo;
      glCreateBuffers(1, &dibo);
      objects[DIBO] = dibo;
    }
  if (flags & TFBO)
    {
      GLuint tfbo;
      glCreateTransformFeedbacks(1, &tfbo);
      objects[TFBO] = tfbo;
    }
}

void OpenglBuffer::deleteBuffers()
{
  if (objects[VAO])
    glDeleteVertexArrays(1, &objects[VAO]);
  if (objects[VBO])
    glDeleteBuffers(1, &objects[VBO]);
  if (objects[INSTANCE_VBO])
    glDeleteBuffers(1, &objects[INSTANCE_VBO]);
  if (objects[EBO])
    glDeleteBuffers(1, &objects[EBO]);
  if (objects[DIBO])
    glDeleteBuffers(1, &objects[DIBO]);
  if (objects[TFBO])
    glDeleteTransformFeedbacks(1, &objects[TFBO]);
}

GLuint &OpenglBuffer::get(int flag)
{
  if (flag & VAO)
    return objects[VAO];
  else if (flag & VBO)
    return objects[VBO];
  else if (flag & INSTANCE_VBO)
    return objects[INSTANCE_VBO];
  else if (flag & EBO)
    return objects[EBO];
  else if (flag & DIBO)
    return objects[DIBO];
  else if (flag & TFBO)
    return objects[TFBO];
  else throw std::invalid_argument("Unknown GL object enum flag");
}

void OpenglBuffer::bind(int flag)
{
  if (flag & VAO)
    glBindVertexArray(objects[VAO]);
  if (flag & VBO)
    glBindBuffer(GL_ARRAY_BUFFER, objects[VBO]);
  if (flag & INSTANCE_VBO)
    glBindBuffer(GL_ARRAY_BUFFER, objects[INSTANCE_VBO]);
  if (flag & EBO)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objects[EBO]);
  if (flag & DIBO)
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, objects[DIBO]);
  if (flag & TFBO)
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, objects[TFBO]);
}

void OpenglBuffer::add(int flag)
{
  if (flag & VAO)
    {
      GLuint vao;
      glCreateVertexArrays(1, &vao);
      objects[VAO] = vao;
    }
  else if (flag & VBO)
    {
      GLuint vbo;
      glCreateBuffers(1, &vbo);
      objects[VBO] = vbo;
    }
  else if (flag & INSTANCE_VBO)
    {
      GLuint vbo;
      glCreateBuffers(1, &vbo);
      objects[INSTANCE_VBO] = vbo;
    }
  else if (flag & EBO)
    {
      GLuint ebo;
      glCreateBuffers(1, &ebo);
      objects[EBO] = ebo;
    }
  else if (flag & DIBO)
    {
      GLuint dibo;
      glCreateBuffers(1, &dibo);
      objects[DIBO] = dibo;
    }
  else if (flag & TFBO)
    {
      GLuint tfbo;
      glCreateTransformFeedbacks(1, &tfbo);
      objects[TFBO] = tfbo;
    }
  else
    throw std::invalid_argument("Unknown GL object enum flag");
}
