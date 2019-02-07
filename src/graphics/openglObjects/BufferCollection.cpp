#include "graphics/openglObjects/BufferCollection.h"

BufferCollection::BufferCollection(int flags)
{
  create(flags);
}

BufferCollection::BufferCollection(BufferCollection &&old) noexcept
{
  if (old.objects[VAO])
    objects[VAO] = old.objects[VAO];
  if (old.objects[VBO])
    objects[VBO] = old.objects[VBO];
  if (old.objects[INSTANCE_VBO])
    objects[INSTANCE_VBO] = old.objects[INSTANCE_VBO];
  if (old.objects[EBO])
    objects[EBO] = old.objects[EBO];
  if (old.objects[DIBO])
    objects[DIBO] = old.objects[DIBO];
  if (old.objects[TFBO])
    objects[TFBO] = old.objects[TFBO];
  old.objects.clear();
}

BufferCollection::BufferCollection(BufferCollection &copy)
{
  if (copy.objects[VAO])
    objects[VAO] = copy.objects[VAO];
  if (copy.objects[VBO])
    objects[VBO] = copy.objects[VBO];
  if (copy.objects[INSTANCE_VBO])
    objects[INSTANCE_VBO] = copy.objects[INSTANCE_VBO];
  if (copy.objects[EBO])
    objects[EBO] = copy.objects[EBO];
  if (copy.objects[DIBO])
    objects[DIBO] = copy.objects[DIBO];
  if (copy.objects[TFBO])
    objects[TFBO] = copy.objects[TFBO];
}

BufferCollection::~BufferCollection()
{
  deleteBuffers();
}

void BufferCollection::bindZero(int flag)
{
  if (flag & VAO)
    glBindVertexArray(0);
  if (flag & VBO || flag & INSTANCE_VBO)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  if (flag & EBO)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  if (flag & DIBO)
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
  if (flag & TFBO)
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}

void BufferCollection::create(int flags)
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

void BufferCollection::deleteBuffers()
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

void BufferCollection::deleteBuffer(int flag)
{
  if (flag & VAO)
    glDeleteVertexArrays(1, &objects[VAO]);
  else if (flag & VBO)
    glDeleteBuffers(1, &objects[VBO]);
  else if (flag & INSTANCE_VBO)
    glDeleteBuffers(1, &objects[INSTANCE_VBO]);
  else if (flag & EBO)
    glDeleteBuffers(1, &objects[EBO]);
  else if (flag & DIBO)
    glDeleteBuffers(1, &objects[DIBO]);
  else if (flag & TFBO)
    glDeleteTransformFeedbacks(1, &objects[TFBO]);
  else
    throw std::invalid_argument("Unknown GL object enum flag");
}

GLuint &BufferCollection::get(int flag)
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
  else
    throw std::invalid_argument("Unknown GL object enum flag");
}

void BufferCollection::bind(int flag)
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

void BufferCollection::add(int flag)
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
