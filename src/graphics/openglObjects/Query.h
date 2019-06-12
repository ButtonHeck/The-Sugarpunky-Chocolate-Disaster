#pragma once

#include <GL/glew.h>

class Query
{
public:
  Query(GLuint type) noexcept;
  virtual ~Query();
  void start() noexcept;
  void end() noexcept;
  bool isResultAvailable() noexcept;
  GLuint requestResult() noexcept;
  GLuint getResult() const noexcept;
  bool isInUse() const noexcept;

private:
  GLuint id;
  GLuint type;
  GLuint result;
  bool inUse = false;
  GLuint resultAvailable;
};
