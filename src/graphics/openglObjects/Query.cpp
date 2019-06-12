#include "Query"

Query::Query(GLuint type) noexcept
  :
    type(type)
{
  glCreateQueries(type, 1, &id);
}

Query::~Query()
{
  glDeleteQueries(1, &id);
}

void Query::start() noexcept
{
  glBeginQuery(type, id);
  inUse = true;
}

void Query::end() noexcept
{
  glEndQuery(type);
}

bool Query::isResultAvailable() noexcept
{
  glGetQueryObjectuiv(id, GL_QUERY_RESULT_AVAILABLE, &resultAvailable);
  return resultAvailable;
}

GLuint Query::requestResult() noexcept
{
  inUse = false;
  glGetQueryObjectuiv(id, GL_QUERY_RESULT, &result);
  return result;
}

GLuint Query::getResult() const noexcept
{
  return result;
}

bool Query::isInUse() const noexcept
{
  return inUse;
}
