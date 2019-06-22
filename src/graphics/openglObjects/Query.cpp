/*
 * Copyright 2019 Ilya Malgin
 * Query.cpp
 * This file is part of The Sugarpunky Chocolate Disaster project
 *
 * The Sugarpunky Chocolate Disaster project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The Sugarpunky Chocolate Disaster project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * See <http://www.gnu.org/licenses/>
 *
 * Purpose: contains definitions for Query class
 * @version 0.1.0
 */

#include "Query"

/**
* @brief plain ctor. Sends create query of a given type command to OpenGL
* @param type GL defined type of this query
*/
Query::Query(GLuint type) noexcept
  :
    type(type)
{
  glCreateQueries(type, 1, &id);
}

/**
* @brief plain dtor. Sends delete query object command to OpenGL
*/
Query::~Query()
{
  glDeleteQueries(1, &id);
}

/**
* @brief sends begin query command to OpenGL
*/
void Query::start() noexcept
{
  glBeginQuery(type, id);
  inUse = true;
}

/**
* @brief sends end query command to OpenGL
*/
void Query::end() noexcept
{
  glEndQuery(type);
}

/**
* @brief tells whether query object result is available
*/
bool Query::isResultAvailable() noexcept
{
  glGetQueryObjectuiv(id, GL_QUERY_RESULT_AVAILABLE, &resultAvailable);
  return resultAvailable;
}

/**
* @brief explicitly tells OpenGL to get query result and returns it
*/
GLuint Query::requestResult() noexcept
{
  inUse = false;
  glGetQueryObjectuiv(id, GL_QUERY_RESULT, &result);
  return result;
}

/**
* @brief similar to request version, but do not forces OpenGL to load result from GPU, 
* it just returns whatever is stored in local variable
*/
GLuint Query::getResult() const noexcept
{
  return result;
}

/**
* @note a query is in 'inUse' state from the client point of view if glBeginQuery call has been made 
* and result of a query has NOT yet been returned by GPU. 
* e.g. glEndQuery without subsequent glGetQueryObjectuiv(result) call does NOT change the 'inUse' state.
* This decision was made on purpose to avoid CPU stalling when GPU is on its business
*/
bool Query::isInUse() const noexcept
{
  return inUse;
}
