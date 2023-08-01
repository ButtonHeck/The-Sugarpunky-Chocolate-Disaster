/*
 * Copyright 2019 Ilya Malgin
 * BufferCollection.cpp
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
 * Purpose: contains definition for BufferCollection class
 * @version 0.1.0
 */

#include "BufferCollection"

#include <stdexcept>

/**
* @brief creating a preset buffer objects pipeline according to given flags
* @param flags integer union of individual flags
*/
BufferCollection::BufferCollection( int flags )
{
	create( flags );
}

/**
* @brief move ctor
* @param old collection to be moved
*/
BufferCollection::BufferCollection( BufferCollection && old ) noexcept
{
	if( old.objects[VAO] )
	{
		objects[VAO] = old.objects[VAO];
	}
	if( old.objects[VBO] )
	{
		objects[VBO] = old.objects[VBO];
	}
	if( old.objects[INSTANCE_VBO] )
	{
		objects[INSTANCE_VBO] = old.objects[INSTANCE_VBO];
	}
	if( old.objects[EBO] )
	{
		objects[EBO] = old.objects[EBO];
	}
	if( old.objects[DIBO] )
	{
		objects[DIBO] = old.objects[DIBO];
	}
	if( old.objects[TFBO] )
	{
		objects[TFBO] = old.objects[TFBO];
	}
	old.objects.clear();
}

/**
* @brief copy ctor
* @param copy collection to be copied from
*/
BufferCollection::BufferCollection( BufferCollection & copy )
{
	if( copy.objects[VAO] )
	{
		objects[VAO] = copy.objects[VAO];
	}
	if( copy.objects[VBO] )
	{
		objects[VBO] = copy.objects[VBO];
	}
	if( copy.objects[INSTANCE_VBO] )
	{
		objects[INSTANCE_VBO] = copy.objects[INSTANCE_VBO];
	}
	if( copy.objects[EBO] )
	{
		objects[EBO] = copy.objects[EBO];
	}
	if( copy.objects[DIBO] )
	{
		objects[DIBO] = copy.objects[DIBO];
	}
	if( copy.objects[TFBO] )
	{
		objects[TFBO] = copy.objects[TFBO];
	}
}

BufferCollection::~BufferCollection()
{
	deleteBuffers();
}

/**
* @brief replaces all bind-to-zero GL calls boilerplate code in one function
* @param flag indicator of the GL object type that should be unbound
*/
void BufferCollection::bindZero( int flag ) noexcept
{
	if( flag & VAO )
	{
		glBindVertexArray( 0 );
	}
	if( flag & VBO || flag & INSTANCE_VBO )
	{
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
	}
	if( flag & EBO )
	{
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	}
	if( flag & DIBO )
	{
		glBindBuffer( GL_DRAW_INDIRECT_BUFFER, 0 );
	}
	if( flag & TFBO )
	{
		glBindTransformFeedback( GL_TRANSFORM_FEEDBACK, 0 );
	}
}

/**
* @brief sends create command to OpenGL side and stores object's ID in the storage
* @param flags integer union of individual flags
*/
void BufferCollection::create( int flags )
{
	if( flags & VAO )
	{
		GLuint vao;
		glCreateVertexArrays( 1, &vao );
		objects[VAO] = vao;
	}
	if( flags & VBO )
	{
		GLuint vbo;
		glCreateBuffers( 1, &vbo );
		objects[VBO] = vbo;
	}
	if( flags & INSTANCE_VBO )
	{
		GLuint vbo;
		glCreateBuffers( 1, &vbo );
		objects[INSTANCE_VBO] = vbo;
	}
	if( flags & EBO )
	{
		GLuint ebo;
		glCreateBuffers( 1, &ebo );
		objects[EBO] = ebo;
	}
	if( flags & DIBO )
	{
		GLuint dibo;
		glCreateBuffers( 1, &dibo );
		objects[DIBO] = dibo;
	}
	if( flags & TFBO )
	{
		GLuint tfbo;
		glCreateTransformFeedbacks( 1, &tfbo );
		objects[TFBO] = tfbo;
	}
}

/**
* @brief sends command to OpenGL to delete each GL object if one exists in a collection
*/
void BufferCollection::deleteBuffers()
{
	if( objects[VAO] )
	{
		glDeleteVertexArrays( 1, &objects[VAO] );
	}
	if( objects[VBO] )
	{
		glDeleteBuffers( 1, &objects[VBO] );
	}
	if( objects[INSTANCE_VBO] )
	{
		glDeleteBuffers( 1, &objects[INSTANCE_VBO] );
	}
	if( objects[EBO] )
	{
		glDeleteBuffers( 1, &objects[EBO] );
	}
	if( objects[DIBO] )
	{
		glDeleteBuffers( 1, &objects[DIBO] );
	}
	if( objects[TFBO] )
	{
		glDeleteTransformFeedbacks( 1, &objects[TFBO] );
	}
}

/**
* @brief sends command to OpenGL to delete a particular GL object
* @param flag indicator of the GL object type that should be deleted
*/
void BufferCollection::deleteBuffer( int flag )
{
	if( flag & VAO )
	{
		glDeleteVertexArrays( 1, &objects[VAO] );
	}
	else if( flag & VBO )
	{
		glDeleteBuffers( 1, &objects[VBO] );
	}
	else if( flag & INSTANCE_VBO )
	{
		glDeleteBuffers( 1, &objects[INSTANCE_VBO] );
	}
	else if( flag & EBO )
	{
		glDeleteBuffers( 1, &objects[EBO] );
	}
	else if( flag & DIBO )
	{
		glDeleteBuffers( 1, &objects[DIBO] );
	}
	else if( flag & TFBO )
	{
		glDeleteTransformFeedbacks( 1, &objects[TFBO] );
	}
	else
	{
		throw std::invalid_argument( "Unknown GL object enum flag" );
	}
}

/**
* @brief return a GL object's ID
* @param flag indicator of the GL object type whose ID should be returned
*/
GLuint & BufferCollection::get( int flag )
{
	if( flag & VAO )
	{
		return objects[VAO];
	}
	else if( flag & VBO )
	{
		return objects[VBO];
	}
	else if( flag & INSTANCE_VBO )
	{
		return objects[INSTANCE_VBO];
	}
	else if( flag & EBO )
	{
		return objects[EBO];
	}
	else if( flag & DIBO )
	{
		return objects[DIBO];
	}
	else if( flag & TFBO )
	{
		return objects[TFBO];
	}
	else
	{
		throw std::invalid_argument( "Unknown GL object enum flag" );
	}
}

/**
* @brief sends bind command to OpenGL for a chosen GL objects
* @param flag indicator of the GL object to be bound
*/
void BufferCollection::bind( int flag )
{
	if( flag & VAO )
	{
		glBindVertexArray( objects[VAO] );
	}
	if( flag & VBO )
	{
		glBindBuffer( GL_ARRAY_BUFFER, objects[VBO] );
	}
	if( flag & INSTANCE_VBO )
	{
		glBindBuffer( GL_ARRAY_BUFFER, objects[INSTANCE_VBO] );
	}
	if( flag & EBO )
	{
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, objects[EBO] );
	}
	if( flag & DIBO )
	{
		glBindBuffer( GL_DRAW_INDIRECT_BUFFER, objects[DIBO] );
	}
	if( flag & TFBO )
	{
		glBindTransformFeedback( GL_TRANSFORM_FEEDBACK, objects[TFBO] );
	}
}

/**
* @brief similar to create method, but intended to be used after collection has been created and suppose to take one type per call
* @param flag indicator of the GL object to be created
*/
void BufferCollection::add( int flag )
{
	if( flag & VAO )
	{
		GLuint vao;
		glCreateVertexArrays( 1, &vao );
		objects[VAO] = vao;
	}
	else if( flag & VBO )
	{
		GLuint vbo;
		glCreateBuffers( 1, &vbo );
		objects[VBO] = vbo;
	}
	else if( flag & INSTANCE_VBO )
	{
		GLuint vbo;
		glCreateBuffers( 1, &vbo );
		objects[INSTANCE_VBO] = vbo;
	}
	else if( flag & EBO )
	{
		GLuint ebo;
		glCreateBuffers( 1, &ebo );
		objects[EBO] = ebo;
	}
	else if( flag & DIBO )
	{
		GLuint dibo;
		glCreateBuffers( 1, &dibo );
		objects[DIBO] = dibo;
	}
	else if( flag & TFBO )
	{
		GLuint tfbo;
		glCreateTransformFeedbacks( 1, &tfbo );
		objects[TFBO] = tfbo;
	}
	else
	{
		throw std::invalid_argument( "Unknown GL object enum flag" );
	}
}
