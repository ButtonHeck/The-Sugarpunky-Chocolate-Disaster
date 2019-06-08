#include "Query"

Query::Query(GLuint type)
  :
    type(type)
{
  glCreateQueries(type, 1, &id);
}

Query::~Query()
{
  glDeleteQueries(1, &id);
}

void Query::start()
{
  glBeginQuery(type, id);
  inUse = true;
}

void Query::end()
{
  glEndQuery(type);
}

bool Query::isResultAvailable()
{
  glGetQueryObjectuiv(id, GL_QUERY_RESULT_AVAILABLE, &resultAvailable);
  return resultAvailable;
}

GLuint Query::requestResult()
{
  inUse = false;
  glGetQueryObjectuiv(id, GL_QUERY_RESULT, &result);
  return result;
}

GLuint Query::getResult() const
{
  return result;
}

bool Query::isInUse() const
{
  return inUse;
}
