#pragma once

#include <GL/glew.h>

class Query
{
public:
  Query(GLuint type);
  virtual ~Query();
  void start();
  void end();
  bool isResultAvailable();
  GLuint requestResult();
  GLuint getResult() const;
  bool isInUse() const;

private:
  GLuint id;
  GLuint type;
  GLuint result;
  bool inUse = false;
  GLuint resultAvailable;
};
