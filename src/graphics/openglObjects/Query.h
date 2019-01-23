#ifndef QUERY_H
#define QUERY_H
#include <GL/glew.h>

class Query
{
public:
  Query(GLuint type);
  virtual ~Query();
  void start();
  void end();
  bool isResultAvailable();
  GLuint getResult();
  bool isInUse() const;
private:
  GLuint id;
  GLuint type;
  GLuint result;
  bool inUse = false;
  GLuint resultAvailable;
};

#endif // QUERY_H
